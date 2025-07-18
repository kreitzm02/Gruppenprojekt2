// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_BossGolemAttack::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<ABossEnemy_Golem>(m_ownerCharacter);
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
}

void UFSM_BossGolemAttack::OnEnter()
{
	Super::OnEnter();

	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
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
			FCollisionShape::MakeSphere(m_thisEnemy->GetAttackRange()),
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

void UFSM_BossGolemAttack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (m_player)
	{
		FVector playerDirection = m_player->GetActorLocation() - m_ownerCharacter->GetActorLocation();
		playerDirection.Z = 0.0f;
		m_ownerCharacter->SetActorRotation(playerDirection.Rotation());
	}
}

void UFSM_BossGolemAttack::OnExit()
{
	Super::OnExit();

	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
