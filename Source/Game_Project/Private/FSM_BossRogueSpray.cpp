// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossRogueSpray.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_BossRogueSpray::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<ABossEnemy_Rogue>(m_ownerCharacter);

	if (!m_thisEnemy) return;

	m_attackAnimation = m_thisEnemy->GetAttackWOReloadAnimation();
}

void UFSM_BossRogueSpray::OnEnter()
{
	Super::OnEnter();

	if (!m_thisEnemy) return;

	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	m_passedTime = 0.0f;
	m_animDuration = 0.0f;

	m_multiplier = m_thisEnemy->GetCurrentMultiplier();

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Spray has no Owner Pawn!"))
	}
	else
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}
}

void UFSM_BossRogueSpray::OnUpdate(float a_deltaTime)
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

	m_passedTime += a_deltaTime * m_slowerMultiplier * m_multiplier;

	if (m_passedTime >= m_animDuration)
	{
		m_animDuration = m_attackAnimation->GetPlayLength();
		m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, false);
		UAnimSingleNodeInstance* singleNodeInstance = m_ownerSkeletalMesh->GetSingleNodeInstance();

		if (singleNodeInstance)
		{
			singleNodeInstance->SetPlayRate(m_slowerMultiplier * m_multiplier);
		}

		m_passedTime = 0.0f;
		m_shotFired = false;
	}

	if (!m_shotFired && m_passedTime >= m_shootAtAnimStartOffset)
	{
		m_thisEnemy->PlayBasicAttackSound(false);
		m_thisEnemy->FireArrow(m_target);
		m_shotFired = true;
	}
}

void UFSM_BossRogueSpray::OnExit()
{
	Super::OnExit();

	m_target = nullptr;
	
	if (!m_thisEnemy) return;

	m_thisEnemy->SetArrowSprayReady(false);
	m_thisEnemy->StopOwnSound();
}