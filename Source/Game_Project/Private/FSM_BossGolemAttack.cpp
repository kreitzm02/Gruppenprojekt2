// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_BossGolemAttack::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<ABossEnemy_Golem>(m_ownerCharacter);
	if (m_thisEnemy)
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
}

void UFSM_BossGolemAttack::OnEnter()
{
	Super::OnEnter();
	if (m_thisEnemy)
	{
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
		m_animationDuration = m_attackAnimation->GetPlayLength();
		m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UFSM_BossGolemAttack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (m_thisEnemy)
	{
		if (m_player)
		{
			FVector playerDirection = m_player->GetActorLocation() - m_ownerCharacter->GetActorLocation();
			playerDirection.Z = 0.0f;
			m_ownerCharacter->SetActorRotation(playerDirection.Rotation());
		}

		m_passedTime += a_deltaTime;

		if (!m_animationStarted)
		{
			m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, false);

			m_passedTime = 0.0f;

			m_animationStarted = true;
		}

		if (!m_soundStarted && m_passedTime >= m_playSoundAtAnimOffset)
		{
			m_thisEnemy->PlayBasicAttackSound(false);
			m_soundStarted = true;
		}

		if (m_animationStarted && m_passedTime >= m_animationDuration)
		{
			m_soundStarted = false;
			m_animationStarted = false;
		}
	}
}

void UFSM_BossGolemAttack::OnExit()
{
	Super::OnExit();

	if (m_thisEnemy)
	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
