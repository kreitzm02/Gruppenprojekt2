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
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_ownerCharacter);

	if (!m_ownerCharacter) return;

	m_chaseAnimation = enemy->GetChaseAnimation();
	m_chaseRange = enemy->GetPlayerChaseRadius(); 
	m_walkSpeed = enemy->GetChaseWalkSpeed();
}

void UFSM_ChasePlayer::OnEnter()
{
    Super::OnEnter();

	if (!m_ownerCharacter) return;

	m_ownerSkeletalMesh->PlayAnimation(m_chaseAnimation, true);

	ACharacter* character = Cast<ACharacter>(m_ownerCharacter);
	character->GetCharacterMovement()->MaxWalkSpeed = m_walkSpeed;
}

void UFSM_ChasePlayer::OnUpdate(float a_deltaTime)
{
    Super::OnUpdate(a_deltaTime);

	if (!m_ownerCharacter) return;

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Chase has no Owner Pawn!"))
	}
	else
	{
		TArray<FOverlapResult> overlaps;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(m_ownerCharacter);

		FCollisionObjectQueryParams objectQueryParams;
		objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

		bool hasOverlap = m_ownerCharacter->GetWorld()->OverlapMultiByObjectType(
			overlaps,
			m_ownerCharacter->GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(m_chaseRange),
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
					AAIController* aiController = Cast<AAIController>(m_ownerCharacter->GetController());
					if (aiController)
					{
						UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(m_ownerCharacter->GetWorld());
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