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
	AEnemy_Warrior* enemy = Cast<AEnemy_Warrior>(m_ownerPawn);
	m_chargeAnimation = enemy->GetChargeAnimation();
	m_chargeSpeed = enemy->GetChargeSpeed();
	m_chaseRange = enemy->GetPlayerChaseRadius();
}

void UFSM_WarriorCharge::OnEnter()
{
	Super::OnEnter();

	m_ownerSkeletalMesh->PlayAnimation(m_chargeAnimation, true);

	if (m_ownerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		TArray<FOverlapResult> overlaps;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(m_ownerPawn);

		FCollisionObjectQueryParams objectQueryParams;
		objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

		bool test = m_ownerPawn->GetWorld()->OverlapMultiByObjectType(
			overlaps,
			m_ownerPawn->GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(m_chaseRange),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			AActor* actor = overlap.GetActor();
			if (actor && actor->IsA(ACharacter::StaticClass()))
			{
				m_player = Cast<ACharacter>(actor);
			}
		}
		ACharacter* character = Cast<ACharacter>(m_ownerPawn);
		character->GetCharacterMovement()->MaxWalkSpeed = m_chargeSpeed;
	}
}

void UFSM_WarriorCharge::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);

	AAIController* aiController = Cast<AAIController>(m_ownerPawn->GetController());
	if (aiController)
	{
		UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(m_ownerPawn->GetWorld());
		if (navSystem)
		{
			FNavLocation navLocation;
			if (navSystem->GetRandomPointInNavigableRadius(m_player->GetActorLocation(), 1.0f, navLocation))
			{
				aiController->MoveToLocation(navLocation.Location);
			}
		}
	}
}

void UFSM_WarriorCharge::OnExit()
{
	Super::OnExit();

	AEnemy_Warrior* enemy = Cast<AEnemy_Warrior>(m_ownerPawn);
	enemy->SetChargeReady(false);

}