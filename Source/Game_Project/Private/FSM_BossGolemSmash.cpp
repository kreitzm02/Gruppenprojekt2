// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemSmash.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Engine/OverlapResult.h"

void UFSM_BossGolemSmash::Initialize()
{
	Super::Initialize();

	m_owner = Cast<ABossEnemy_Golem>(m_ownerCharacter);
	m_smashAnimation = m_owner->GetSmashAnimation();
	m_detectionRange = m_owner->GetPlayerChaseRadius();
	m_animationLength = m_smashAnimation->GetPlayLength();
}

void UFSM_BossGolemSmash::OnEnter()
{
	Super::OnEnter();

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

		bool hasOverlap = m_ownerCharacter->GetWorld()->OverlapMultiByObjectType(
			overlaps,
			m_ownerCharacter->GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(m_detectionRange),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			AActor* actor = overlap.GetActor();
			if (actor && actor->IsA(ACharacter::StaticClass()))
			{
				m_player = Cast<ACharacter>(actor);
			}
		}
	}

	m_currentMultiplier = m_owner->GetMultiplier();

	m_passedTime = 0.0f;

	m_shockwaveStarted = false;

	m_ownerSkeletalMesh->PlayAnimation(m_smashAnimation, false);

	if (UAnimSingleNodeInstance* node = m_ownerSkeletalMesh->GetSingleNodeInstance())
	{
		node->SetPlayRate(m_currentMultiplier);
	}
}

void UFSM_BossGolemSmash::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	m_passedTime += a_deltaTime * m_currentMultiplier;

	if (m_passedTime >= m_shockwaveAtAnimStartOffset && !m_shockwaveStarted)
	{
		m_owner->CreateSmashShockwaveWithBoulder(m_player);
		m_shockwaveStarted = true;
	}

	if (m_passedTime >= m_animationLength)
	{
		m_passedTime = 0.0f;

		m_ownerSkeletalMesh->PlayAnimation(m_smashAnimation, false);

		if (UAnimSingleNodeInstance* node = m_ownerSkeletalMesh->GetSingleNodeInstance())
		{
			node->SetPlayRate(m_currentMultiplier);
		}

		m_shockwaveStarted = false;

		m_owner->AddUsedSmash();
	}
}

void UFSM_BossGolemSmash::OnExit()
{
	Super::OnExit();

	m_owner->ResetUsedSmash();

	m_owner->SetSmashReady(false);
}