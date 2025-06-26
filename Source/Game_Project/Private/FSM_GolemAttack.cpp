// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_GolemAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_GolemAttack::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<AEnemy_Golem>(m_ownerPawn);
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
}

void UFSM_GolemAttack::OnEnter()
{
	Super::OnEnter();

	if (ACharacter* character = Cast<ACharacter>(m_ownerPawn))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	if (m_ownerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		AEnemy_Golem* thisEnemy = Cast<AEnemy_Golem>(m_ownerPawn);

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
			FCollisionShape::MakeSphere(thisEnemy->GetPlayerChaseRadius()),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			m_player = overlap.GetActor();
			break;
		}
	}
	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, true);
}

void UFSM_GolemAttack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (m_player)
	{
		FVector playerDirection = m_player->GetActorLocation() - m_ownerPawn->GetActorLocation();
		playerDirection.Z = 0.0f;
		m_ownerPawn->SetActorRotation(playerDirection.Rotation());
	}
}

void UFSM_GolemAttack::OnExit()
{
	Super::OnExit();

	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
