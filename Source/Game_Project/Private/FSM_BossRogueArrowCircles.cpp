// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossRogueArrowCircles.h"
#include "AIController.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSM_BossRogueArrowCircles::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<ABossEnemy_Rogue>(m_ownerCharacter);
	m_attackAnimation = m_thisEnemy->GetAttackWOReloadAnimation();
	m_spawnPoint = m_thisEnemy->GetActorLocation();
	UE_LOG(LogTemp,Warning,TEXT("boss spawnpoint is: %f  %f  %f"), m_spawnPoint.X, m_spawnPoint.Y, m_spawnPoint.Z)
}

void UFSM_BossRogueArrowCircles::OnEnter()
{
	Super::OnEnter();

	m_spawnPoint.Z = m_thisEnemy->GetActorLocation().Z;
	UE_LOG(LogTemp, Warning, TEXT("boss spawnpoint is: %f  %f  %f"), m_spawnPoint.X, m_spawnPoint.Y, m_spawnPoint.Z)

	m_passedTime = 0.0f;
	m_animDuration = 0.0f;

	m_multiplier = m_thisEnemy->GetCurrentMultiplier();

	m_thisEnemy->GetCharacterMovement()->MaxWalkSpeed *= m_multiplier;

	m_ownerSkeletalMesh->PlayAnimation(m_thisEnemy->GetChaseAnimation(), true);

	if (AAIController* AIController = Cast<AAIController>(m_thisEnemy->GetController()))
	{
		AIController->MoveToLocation(m_spawnPoint);

	}
}

void UFSM_BossRogueArrowCircles::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if(!m_movedToSpawn)
	{
		float distance = FVector::Dist(m_thisEnemy->GetActorLocation(), m_spawnPoint);

		if (distance <= 150.0f)
		{
			m_movedToSpawn = true;
			m_thisEnemy->GetCharacterMovement()->StopMovementImmediately();
			m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, false);
			m_shootDirection = FMath::VRand();
			m_shootDirection.Z = 0;
			m_shootDirection.Normalize();
			m_thisEnemy->SetActorRotation(m_shootDirection.Rotation());
		}
	}
	else
	{
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

			m_shootDirection = FMath::VRand();
			m_shootDirection.Z = 0;
			m_shootDirection.Normalize();
			m_thisEnemy->SetActorRotation(m_shootDirection.Rotation());

			m_passedTime = 0.0f;
			m_shotFired = false;
		}

		if (!m_shotFired && m_passedTime >= m_shootAtAnimStartOffset)
		{
			m_thisEnemy->FireArrow(m_shootDirection);
			m_shotFired = true;
		}
	}

}

void UFSM_BossRogueArrowCircles::OnExit()
{
	m_movedToSpawn = false;
	m_thisEnemy->SetArrowCircleReady(false);
	m_thisEnemy->GetCharacterMovement()->MaxWalkSpeed = m_thisEnemy->GetChaseWalkSpeed();
	Super::OnExit();
}


