// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_RogueAttack.h"
#include "Enemy_Rogue.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"


void UFSM_RogueAttack::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_Rogue>(m_ownerPawn);
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
	m_reloadAnimation = m_thisEnemy->GetReloadAnimation();
}

void UFSM_RogueAttack::OnEnter()
{
	Super::OnEnter();

	if (ACharacter* character = Cast<ACharacter>(m_ownerPawn))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	m_passedTime = 0.0f;
	m_animDuration = 0.0f;

	if (m_ownerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		AEnemy_Rogue* thisEnemy = Cast<AEnemy_Rogue>(m_ownerPawn);

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
			FCollisionShape::MakeSphere(thisEnemy->GetAttackRange()),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			m_player = overlap.GetActor();
			break;
		}
	}
}

void UFSM_RogueAttack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	FVector playerDirection = m_player->GetActorLocation() - m_ownerPawn->GetActorLocation();
	playerDirection.Z = 0.0f;
	m_ownerPawn->SetActorRotation(playerDirection.Rotation());

	m_passedTime += a_deltaTime;

	if (m_weaponLoaded && m_passedTime >= m_animDuration)
	{
		m_animDuration = m_attackAnimation->GetPlayLength();
		m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, false);

		m_passedTime = 0.0f;

		m_weaponLoaded = false;
		m_shotFired = false;
	}
	else if (!m_weaponLoaded && m_passedTime >= m_animDuration)
	{
		m_animDuration = m_reloadAnimation->GetPlayLength();
		m_ownerSkeletalMesh->PlayAnimation(m_reloadAnimation, false);

		m_passedTime = 0.0f;

		m_weaponLoaded = true;
	}

	if (!m_shotFired && m_passedTime >= m_shootAtAnimStartOffset)
	{
		m_thisEnemy->FireArrow(FVector::ForwardVector);
		m_shotFired = true;
	}
}

void UFSM_RogueAttack::OnExit()
{
	Super::OnExit();
}
