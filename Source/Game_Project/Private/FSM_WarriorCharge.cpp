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
	AEnemy_Warrior* enemy = Cast<AEnemy_Warrior>(ownerPawn);
	chargeAnimation = enemy->GetChargeAnimation();
	chargeSpeed = enemy->GetChargeSpeed();
	chaseRange = enemy->GetPlayerChaseRadius();
}

void UFSM_WarriorCharge::OnEnter()
{
	Super::OnEnter();

	ownerSkeletalMesh->PlayAnimation(chargeAnimation, true);

	if (ownerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		TArray<FOverlapResult> overlaps;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(ownerPawn);

		FCollisionObjectQueryParams objectQueryParams;
		objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

		bool test = ownerPawn->GetWorld()->OverlapMultiByObjectType(
			overlaps,
			ownerPawn->GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(chaseRange),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			AActor* actor = overlap.GetActor();
			if (actor && actor->IsA(ACharacter::StaticClass()))
			{
				player = Cast<ACharacter>(actor);
			}
		}
		ACharacter* character = Cast<ACharacter>(ownerPawn);
		character->GetCharacterMovement()->MaxWalkSpeed = chargeSpeed;
	}
}

void UFSM_WarriorCharge::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);

	AAIController* aiController = Cast<AAIController>(ownerPawn->GetController());
	if (aiController)
	{
		UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(ownerPawn->GetWorld());
		if (navSystem)
		{
			FNavLocation navLocation;
			if (navSystem->GetRandomPointInNavigableRadius(player->GetActorLocation(), 1.0f, navLocation))
			{
				aiController->MoveToLocation(navLocation.Location);
			}
		}
	}
}

void UFSM_WarriorCharge::OnExit()
{
	Super::OnExit();

	AEnemy_Warrior* enemy = Cast<AEnemy_Warrior>(ownerPawn);
	enemy->SetChargeReady(false);

}