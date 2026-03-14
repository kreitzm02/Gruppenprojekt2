// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_WarriorCharge.h"
#include "AIController.h"
#include "Enemy_Warrior.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_WarriorCharge::Initialize()
{
	Super::Initialize();
	AEnemy_Warrior* enemy = Cast<AEnemy_Warrior>(m_ownerCharacter);

	if (!enemy) return;

	m_chargeAnimation = enemy->GetChargeAnimation();
	m_chargeSpeed = enemy->GetChargeSpeed();
	m_chaseRange = enemy->GetPlayerChaseRadius();
}

void UFSM_WarriorCharge::OnEnter()
{
	Super::OnEnter();

	if (!m_ownerCharacter) return;

	m_ownerSkeletalMesh->PlayAnimation(m_chargeAnimation, true);

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}
}

void UFSM_WarriorCharge::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);

	if (!m_ownerCharacter) return;
	
	if (m_target == nullptr)
	{
		m_target = Cast<AEnemyCharacter>(m_ownerCharacter)->GetCurrentTarget();
	}
	
	if (m_target == nullptr)
	{
		return;
	}

	AAIController* aiController = Cast<AAIController>(m_ownerCharacter->GetController());
	if (aiController)
	{
		UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(m_ownerCharacter->GetWorld());
		if (navSystem)
		{
			FNavLocation navLocation;
			if (navSystem->GetRandomPointInNavigableRadius(m_target->GetActorLocation(), 1.0f, navLocation))
			{
				aiController->MoveToLocation(navLocation.Location);
			}
		}
	}
}

void UFSM_WarriorCharge::OnExit()
{
	Super::OnExit();
	
	m_target = nullptr;
	
	if (!m_ownerCharacter) return;

	AEnemy_Warrior* enemy = Cast<AEnemy_Warrior>(m_ownerCharacter);
	enemy->SetChargeReady(false);

}