// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossMageFireball.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"


void UFSM_BossMageFireball::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<ABossEnemy_Mage>(m_ownerCharacter);
	m_castShootAnimation = m_thisEnemy->GetFireballCastAnimation();
	m_castingAnimation = m_thisEnemy->GetFireballCastingAnimation();
	m_thisEnemy->SetFireballCastTime(m_castingAnimation->GetPlayLength() * m_castingLoopsUntilFireball + m_castShootAnimation->GetPlayLength());

	m_castDuration = m_castingAnimation->GetPlayLength() * m_castingLoopsUntilFireball;
}

void UFSM_BossMageFireball::OnEnter()
{
	Super::OnEnter();

	m_ownerCharacter->GetCharacterMovement()->StopMovementImmediately();

	m_passedTime = 0.0f;

	m_castShootStarted = false;

	m_fireballFired = false;

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Fireball has no Owner Pawn!"))
	}
	else
	{
		ABossEnemy_Mage* thisEnemy = Cast<ABossEnemy_Mage>(m_ownerCharacter);

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
			FCollisionShape::MakeSphere(thisEnemy->GetPlayerChaseRadius()),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			m_player = overlap.GetActor();
			break;
		}
	}

	m_ownerSkeletalMesh->PlayAnimation(m_castingAnimation, true);
}

void UFSM_BossMageFireball::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	FVector playerDirection = m_player->GetActorLocation() - m_ownerCharacter->GetActorLocation();
	playerDirection.Z = 0.0f;
	m_ownerCharacter->SetActorRotation(playerDirection.Rotation());

	m_passedTime += a_deltaTime;

	if (m_passedTime >= m_castDuration && !m_castShootStarted)
	{

		m_ownerSkeletalMesh->PlayAnimation(m_castShootAnimation, false);

		m_castShootStarted = true;
	}

	if (!m_fireballFired && m_passedTime >= m_castDuration + m_shootAtAnimStartOffset)
	{
		m_thisEnemy->PlayCastFireballSound(false);
		m_thisEnemy->FireFireball(m_player);
		m_fireballFired = true;
	}
}

void UFSM_BossMageFireball::OnExit()
{
	Super::OnExit();
	m_thisEnemy->StopOwnSound();
	m_thisEnemy->SetFireballReady(false);
}
