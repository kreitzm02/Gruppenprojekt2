// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_RogueSpray.h"
#include "Enemy_Rogue.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_RogueSpray::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<AEnemy_Rogue>(m_ownerCharacter);

	if (!m_thisEnemy) return;

	m_attackAnimation = m_thisEnemy->GetArrowSprayAnimation();
}

void UFSM_RogueSpray::OnEnter()
{
	Super::OnEnter();

	if (!m_thisEnemy) return;

	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	m_passedTime = 0.0f;
	m_animDuration = 0.0f;

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Spray has no Owner Pawn!"))
	}
	else
	{

		TArray<FOverlapResult> overlaps;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(m_ownerCharacter);

		FCollisionObjectQueryParams objectQueryParams;
		objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

		bool test = m_ownerCharacter->GetWorld()->OverlapMultiByObjectType(
			overlaps,
			m_ownerCharacter->GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(m_thisEnemy->GetPlayerChaseRadius()),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			m_player = overlap.GetActor();
			break;
		}
	}
}

void UFSM_RogueSpray::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (!m_thisEnemy) return;

	if (m_player)
	{
		FVector playerDirection = m_player->GetActorLocation() - m_ownerCharacter->GetActorLocation();
		playerDirection.Z = 0.0f;
		m_ownerCharacter->SetActorRotation(playerDirection.Rotation());
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
		m_thisEnemy->PlayBasicAttackSound(false);
		m_thisEnemy->FireArrow(m_player);
		m_shotFired = true;
	}
}

void UFSM_RogueSpray::OnExit()
{
	Super::OnExit();

	if (!m_thisEnemy) return;

	m_thisEnemy->SetArrowSprayReady(false);
	m_thisEnemy->StopOwnSound();
	
}