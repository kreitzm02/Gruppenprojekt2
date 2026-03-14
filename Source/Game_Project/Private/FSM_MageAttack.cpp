// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_MageAttack.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_MageAttack::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<AEnemy_Mage>(m_ownerCharacter);

	if (!m_thisEnemy) return;

	m_attackAnimation = m_thisEnemy->GetAttackAnimation();

	m_animDuration = m_attackAnimation->GetPlayLength();
}

void UFSM_MageAttack::OnEnter()
{
	Super::OnEnter();

	if (!m_thisEnemy) return;

	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	m_passedTime = 0.0f;
	m_animDuration = 0.0f;

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}
}

void UFSM_MageAttack::OnUpdate(float a_deltaTime)
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
	
	if (m_target)
	{
		FVector playerDirection = m_target->GetActorLocation() - m_ownerCharacter->GetActorLocation();
		playerDirection.Z = 0.0f;
		m_ownerCharacter->SetActorRotation(playerDirection.Rotation());
	}


	m_passedTime += a_deltaTime;

	if (m_passedTime >= m_animDuration)
	{
		m_animDuration = m_attackAnimation->GetPlayLength();
		m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, false);

		m_passedTime = 0.0f;

		m_shotFired = false;
	}

	if (!m_shotFired && m_passedTime >= m_shootAtAnimStartOffset)
	{
		m_thisEnemy->PlayBasicAttackSound(false);
		m_thisEnemy->FireProjectile(m_target);
		m_shotFired = true;
	}
}

void UFSM_MageAttack::OnExit()
{
	Super::OnExit();

	m_target = nullptr;
	
	if (!m_thisEnemy) return;

	m_thisEnemy->StopOwnSound();
}