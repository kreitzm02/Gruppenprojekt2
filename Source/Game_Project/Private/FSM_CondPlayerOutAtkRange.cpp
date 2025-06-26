// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_CondPlayerOutAtkRange.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "EnemyCharacter.h"
#include "Engine/OverlapResult.h"


void UFSM_CondPlayerOutAtkRange::Initialize()
{
	m_owner = Cast<AActor>(GetOuter()->GetOuter());

	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_owner);
	m_attackRange = enemy->GetAttackRange();
	m_attackDuration = enemy->GetAttackDuration();
}

void UFSM_CondPlayerOutAtkRange::ResetCondition()
{
	m_passedTime = 0.0f;
}

bool UFSM_CondPlayerOutAtkRange::IsConditionMet(float a_deltaTime)
{
	m_passedTime += a_deltaTime;

	if (m_owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("In Range Cond AActor not found!"))
			return false;
	}
	TArray<FOverlapResult> overlaps;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(m_owner);

	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

	bool hasOverlap = m_owner->GetWorld()->OverlapMultiByObjectType(
		overlaps,
		m_owner->GetActorLocation(),
		FQuat::Identity,
		objectQueryParams,
		FCollisionShape::MakeSphere(m_attackRange),
		queryParams
	);

	DrawDebugSphere(
		m_owner->GetWorld(),
		m_owner->GetActorLocation(),
		m_attackRange,
		16,
		FColor::Red,
		false,
		0.1f,
		0,
		1.0f
	);

	if (!hasOverlap && m_passedTime >= m_attackDuration)
	{
		return true;
	}
	else if (hasOverlap && m_passedTime >= m_attackDuration)
	{
		m_passedTime = 0.0f;
		return false;
	}

	return false;
}
