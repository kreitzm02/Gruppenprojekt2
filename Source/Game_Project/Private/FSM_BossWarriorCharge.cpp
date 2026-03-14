// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorCharge.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"


void UFSM_BossWarriorCharge::Initialize()
{
	Super::Initialize();
	ABossEnemy_Warrior* enemy = Cast<ABossEnemy_Warrior>(m_ownerCharacter);

	if (!enemy) return;

	m_chargeAnimation = enemy->GetChargeAnimation();
	m_chargeSpeed = enemy->GetChargeSpeed();
	m_detectionRange = enemy->GetPlayerChaseRadius();
	m_chargeOnPointDuration = enemy->GetChargeOnPointDuration();
	m_owner = enemy;
}

void UFSM_BossWarriorCharge::OnEnter()
{
	Super::OnEnter();

	if (!m_owner) return;

	m_ownerSkeletalMesh->PlayAnimation(m_chargeAnimation, true);

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
		
		m_owner->GetCharacterMovement()->MaxWalkSpeed = m_chargeSpeed * m_owner->GetMultiplier();
		m_owner->GetCharacterMovement()->MaxAcceleration = 10000;
		m_owner->GetCharacterMovement()->BrakingDecelerationWalking = 10000;
	}

	m_resetChargeDirection = true;
	m_isCharging = false;
	m_passedOnPointTime = 0.0f;
}

void UFSM_BossWarriorCharge::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);

	if (!m_owner) return;

	if (m_target == nullptr)
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}
	
	if (m_target == nullptr)
	{
		return;
	}
	
	if(m_isCharging)
	{
		m_ownerCharacter->AddMovementInput(m_chargeDirectionNormal);


		FHitResult hitResult;
		FVector start = m_ownerCharacter->GetActorLocation();
		FVector end = m_ownerCharacter->GetActorLocation() + (m_chargeDirectionNormal * m_stopBeforeWallMultiplier);

		FCollisionQueryParams traceParams;
		traceParams.AddIgnoredActor(m_ownerCharacter);
		traceParams.AddIgnoredActor(m_target);

		bool hit = m_ownerCharacter->GetWorld()->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			ECC_Visibility,
			traceParams
		);

		if (m_enableDebug)
		{
			DrawDebugLine(
				m_ownerCharacter->GetWorld(),
				start,
				end,
				hit ? FColor::Red : FColor::Green,
				false,
				0.1f,
				0,
				1.0f
			);
		}

		if (hit)
		{
			m_resetChargeDirection = true;
			m_isCharging = false;
			m_owner->AddUsedCharge();
		}
	}
	else
	{
		m_passedOnPointTime += a_deltatime * m_owner->GetMultiplier();

		if (m_resetChargeDirection)
		{
			m_chargeDirectionNormal = (m_target->GetActorLocation() - m_ownerCharacter->GetActorLocation()).GetSafeNormal();

			m_chargeDirectionNormal.Z = 0.0f;
			m_owner->SetActorRotation(m_chargeDirectionNormal.Rotation());

			//m_owner->PlayChargingSound(true);

			m_resetChargeDirection = false;
		}
		if (m_passedOnPointTime >= m_chargeOnPointDuration)
		{
			m_owner->GetCharacterMovement()->MaxWalkSpeed = m_chargeSpeed * m_owner->GetMultiplier();
			m_passedOnPointTime = 0.0f;

			m_owner->StopOwnSound();

			m_isCharging = true;
		}
	}
}

void UFSM_BossWarriorCharge::OnExit()
{
	Super::OnExit();

	m_target = nullptr;
	
	if (!m_owner) return;

	m_owner->SetChargeReady(false);
	m_owner->ResetUsedCharges();
}