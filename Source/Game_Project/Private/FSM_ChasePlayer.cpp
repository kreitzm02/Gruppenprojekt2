// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ChasePlayer.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyCharacter.h"
#include "Engine/OverlapResult.h"


void UFSM_ChasePlayer::Initialize()
{
	Super::Initialize();
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(ownerPawn);
	chaseAnimation = enemy->GetChaseAnimation();
	chaseRange = enemy->GetPlayerChaseRadius(); 
	walkSpeed = enemy->GetChaseWalkSpeed();
}

void UFSM_ChasePlayer::OnEnter()
{
    Super::OnEnter();

	ownerSkeletalMesh->PlayAnimation(chaseAnimation, true);

	ACharacter* character = Cast<ACharacter>(ownerPawn);
	character->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void UFSM_ChasePlayer::OnUpdate(float a_deltaTime)
{
    Super::OnUpdate(a_deltaTime);
	if (ownerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Chase has no Owner Pawn!"))
	}
	else
	{
		TArray<FOverlapResult> overlaps;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(ownerPawn);

		FCollisionObjectQueryParams objectQueryParams;
		objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

		bool hasOverlap = ownerPawn->GetWorld()->OverlapMultiByObjectType(
			overlaps,
			ownerPawn->GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(chaseRange),
			queryParams
		);
		for(FOverlapResult& overlap : overlaps)
		{
			AActor* actor = overlap.GetActor();
			if (actor && actor->IsA(ACharacter::StaticClass()))
			{
				ACharacter* player = Cast<ACharacter>(actor);
				if (player)
				{
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
			}
		}
	}

}

void UFSM_ChasePlayer::OnExit()
{
    Super::OnExit();
}