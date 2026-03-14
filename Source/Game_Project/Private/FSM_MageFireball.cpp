// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_MageFireball.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"


void UFSM_MageFireball::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_Mage>(m_ownerCharacter);

	if (!m_thisEnemy) return;

	m_castShootAnimation = m_thisEnemy->GetCastShootAnimation();
	m_castAnimation = m_thisEnemy->GetCastingAnimation();
	m_thisEnemy->SetFireballCastTime(m_castAnimation->GetPlayLength() * m_castingLoopsUntilFireball + m_castShootAnimation->GetPlayLength());

	m_castDuration = m_castAnimation->GetPlayLength() * m_castingLoopsUntilFireball;
}

void UFSM_MageFireball::OnEnter()
{
	Super::OnEnter();

	if (!m_thisEnemy) return;

	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	m_passedTime = 0.0f;

	m_castShootStarted = false;

	m_fireballFired = false;

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Fireball has no Owner Pawn!"))
	}
	else
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}

	m_ownerSkeletalMesh->PlayAnimation(m_castAnimation, true);
}

void UFSM_MageFireball::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (!m_thisEnemy) return;
	
	if (m_target == nullptr)
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}
	
	if (m_target == nullptr)
	{
		return;
	}

	FVector playerDirection = m_target->GetActorLocation() - m_ownerCharacter->GetActorLocation();
	playerDirection.Z = 0.0f;
	m_ownerCharacter->SetActorRotation(playerDirection.Rotation());

	m_passedTime += a_deltaTime;

	if (m_passedTime >= m_castDuration && !m_castShootStarted)
	{
		
		m_ownerSkeletalMesh->PlayAnimation(m_castShootAnimation, false);

		m_castShootStarted = true;
	}

	if (!m_fireballFired && m_passedTime >= m_castDuration + m_shootAtAnimStartOffset)
	{
		m_thisEnemy->PlayCastFireballSound(false);
		m_thisEnemy->FireFireball(m_target);
		m_fireballFired = true;
	}
}

void UFSM_MageFireball::OnExit()
{
	Super::OnExit();

	m_target = nullptr;
	
	if (!m_thisEnemy) return;

	m_thisEnemy->StopOwnSound();
	m_thisEnemy->SetAbilityReady(false);
}

