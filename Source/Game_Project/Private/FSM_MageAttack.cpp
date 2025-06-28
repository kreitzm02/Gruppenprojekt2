// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_MageAttack.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_MageAttack::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<AEnemy_Mage>(m_ownerPawn);
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();

	m_animDuration = m_attackAnimation->GetPlayLength();
}

void UFSM_MageAttack::OnEnter()
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
			FCollisionShape::MakeSphere(m_thisEnemy->GetAttackRange()),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			m_player = overlap.GetActor();
			break;
		}
	}
}

void UFSM_MageAttack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (m_player)
	{
		FVector playerDirection = m_player->GetActorLocation() - m_ownerPawn->GetActorLocation();
		playerDirection.Z = 0.0f;
		m_ownerPawn->SetActorRotation(playerDirection.Rotation());
	}


	m_passedTime += a_deltaTime;

	if (m_passedTime >= m_animDuration)
	{
		m_animDuration = m_attackAnimation->GetPlayLength();
		m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, false);

		m_passedTime = 0.0f;

		m_shotFired = false;
	}

	if (!m_shotFired && m_passedTime >= m_shootAtAnimStartOffset)
	{
		m_thisEnemy->FireProjectile(m_player);
		m_shotFired = true;
	}
}

void UFSM_MageAttack::OnExit()
{
	Super::OnExit();
}