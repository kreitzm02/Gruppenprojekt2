// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemJump.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_BossGolemJump::Initialize()
{
	Super::Initialize();

	ABossEnemy_Golem* enemy = Cast<ABossEnemy_Golem>(m_ownerCharacter);
	if (enemy)
	{
		m_jumpAnimation = enemy->GetJumpAnimation();
		m_detectionRange = enemy->GetPlayerChaseRadius();
		m_animationLength = m_jumpAnimation->GetPlayLength();
		m_owner = enemy;
	}
}

void UFSM_BossGolemJump::OnEnter()
{
	Super::OnEnter();
	if (m_owner)
	{
		if (m_ownerCharacter == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
		}
		else
		{
			m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
		}

		m_currentMultiplier = m_owner->GetMultiplier();

		m_ownerSkeletalMesh->PlayAnimation(m_jumpAnimation, true);

		if (UAnimSingleNodeInstance* node = m_ownerSkeletalMesh->GetSingleNodeInstance())
		{
			node->SetPlayRate(m_currentMultiplier);
		}

		m_isJumping = true;

		m_jumpStartLocation = m_owner->GetActorLocation();

		m_jumpToLocation = m_target->GetActorLocation();

		m_ellapsedTime = 0.0f;

		m_owner->PlayJumpSound(false);

		m_owner->SetActorRotation((m_target->GetActorLocation() - m_owner->GetActorLocation()).Rotation());
	}
}

void UFSM_BossGolemJump::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);
	if (m_owner)
	{
		if (m_target == nullptr)
		{
			m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
		}
	
		if (m_target == nullptr)
		{
			return;
		}
		
		if (m_isJumping)
		{
			m_ellapsedTime += a_deltatime * m_currentMultiplier;
			float alpha = m_ellapsedTime / m_animationLength;

			alpha = FMath::Clamp(alpha, 0.0f, 1.0f);

			if (alpha >= 1.0f)
			{
				m_isJumping = false;

				m_owner->CreateJumpShockwave();

				m_owner->AddUsedJump();
			}
			else
			{
				FVector position = FMath::Lerp(m_jumpStartLocation, m_jumpToLocation, alpha);

				position.Z = FMath::Sin(alpha * PI) * m_jumpHeight + m_owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

				m_owner->SetActorLocation(position);
			}


		}
		else
		{
			m_jumpStartLocation = m_owner->GetActorLocation();

			m_jumpToLocation = m_target->GetActorLocation();

			m_isJumping = true;

			m_ellapsedTime = 0.0f;

			m_owner->PlayJumpSound(false);

			m_owner->SetActorRotation((m_target->GetActorLocation() - m_owner->GetActorLocation()).Rotation());
		}
	}

}

void UFSM_BossGolemJump::OnExit()
{
	Super::OnExit();
	
	m_target = nullptr;
	
	if (m_owner)
	{
		m_owner->ResetUsedJump();
		m_owner->StopOwnSound();
		m_owner->SetJumpReady(false);
	}
}