// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_MageFireball.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"


void UFSM_MageFireball::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_Mage>(m_ownerPawn);
	m_castShootAnimation = m_thisEnemy->GetCastShootAnimation();
	m_castAnimation = m_thisEnemy->GetCastingAnimation();
	m_thisEnemy->SetFireballCastTime(m_castAnimation->GetPlayLength() * m_castingLoopsUntilFireball + m_castShootAnimation->GetPlayLength());

	m_castDuration = m_castAnimation->GetPlayLength() * m_castingLoopsUntilFireball;
}

void UFSM_MageFireball::OnEnter()
{
	Super::OnEnter();

	if (ACharacter* character = Cast<ACharacter>(m_ownerPawn))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	m_passedTime = 0.0f;

	m_castShootStarted = false;

	m_fireballFired = false;

	if (m_ownerPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Fireball has no Owner Pawn!"))
	}
	else
	{
		AEnemy_Mage* thisEnemy = Cast<AEnemy_Mage>(m_ownerPawn);

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

	m_ownerSkeletalMesh->PlayAnimation(m_castAnimation, true);
}

void UFSM_MageFireball::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	FVector playerDirection = m_player->GetActorLocation() - m_ownerPawn->GetActorLocation();
	playerDirection.Z = 0.0f;
	m_ownerPawn->SetActorRotation(playerDirection.Rotation());

	m_passedTime += a_deltaTime;

	if (m_passedTime >= m_castDuration && !m_castShootStarted)
	{
		
		m_ownerSkeletalMesh->PlayAnimation(m_castShootAnimation, false);

		m_castShootStarted = true;
	}

	if (!m_fireballFired && m_passedTime >= m_castDuration + m_shootAtAnimStartOffset)
	{
		m_thisEnemy->FireFireball(Cast<ACharacter>(m_player));
		m_fireballFired = true;
	}
}

void UFSM_MageFireball::OnExit()
{
	Super::OnExit();
	m_thisEnemy->SetAbilityReady(false);
}

