// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossMageBurnGround.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_BossMageBurnGround::Initialize()
{
	Super::Initialize();

	m_owner = Cast<ABossEnemy_Mage>(m_ownerCharacter);

	if (!m_owner) return;

	m_castBurnGroundAnimation = m_owner->GetBurnGroundAnimation();
	m_detectionRange = m_owner->GetPlayerChaseRadius();
	m_animationLength = m_castBurnGroundAnimation->GetPlayLength();
}

void UFSM_BossMageBurnGround::OnEnter()
{
	Super::OnEnter();

	if (!m_owner) return;

	m_ownerCharacter->GetCharacterMovement()->StopMovementImmediately();
	

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}

	m_currentMultiplier = m_owner->GetMultiplier();

	m_passedTime = 0.0f;

	m_burnGroundFired = false;

	m_ownerSkeletalMesh->PlayAnimation(m_castBurnGroundAnimation, false);

	if (UAnimSingleNodeInstance* node = m_ownerSkeletalMesh->GetSingleNodeInstance())
	{
		node->SetPlayRate(m_currentMultiplier);
	}
}

void UFSM_BossMageBurnGround::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (!m_owner) return;
	
	if (m_target == nullptr)
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}
	
	if (m_target == nullptr)
	{
		return;
	}

	m_passedTime += a_deltaTime * m_currentMultiplier;

	if (m_passedTime >= m_burnGroundAtAnimStartOffset && !m_burnGroundFired)
	{
		//fire burn ground
		m_owner->PlayCastBurnGroundSound(false);
		m_owner->FireBurnGround(m_target);

		m_burnGroundFired = true;
	}

	if (m_passedTime >= m_animationLength)
	{
		m_passedTime = 0.0f;

		m_ownerSkeletalMesh->PlayAnimation(m_castBurnGroundAnimation, false);

		if (UAnimSingleNodeInstance* node = m_ownerSkeletalMesh->GetSingleNodeInstance())
		{
			node->SetPlayRate(m_currentMultiplier);
		}

		m_burnGroundFired = false;

		m_owner->AddUsedBurnGround();
	}
}

void UFSM_BossMageBurnGround::OnExit()
{
	Super::OnExit();

	m_target = nullptr;

	if (!m_owner) return;

	m_owner->StopOwnSound();

	m_owner->ResetUsedBurnGrounds();
	
	m_owner->SetBurnGroundReady(false);
}
