// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionWarriorChargeEnd.h"
#include "Engine/OverlapResult.h"

void UFSM_ConditionWarriorChargeEnd::Initialize()
{
	Super::Initialize();
	//owner = Cast<AActor>(GetOuter()->GetOuter());
	m_thisWarrior = Cast<AEnemy_Warrior>(GetOuter()->GetOuter());
	m_attackRange = m_thisWarrior->GetAttackRange();
	m_chargeDuration = m_thisWarrior->GetChargeDuration();
}

void UFSM_ConditionWarriorChargeEnd::ResetCondition()
{
	Super::ResetCondition();
	m_chargeCurrentDuration = 0.0f;
}

bool UFSM_ConditionWarriorChargeEnd::IsConditionMet(float a_deltaTime)
{
	m_chargeCurrentDuration += a_deltaTime;

	TArray<FOverlapResult> overlaps;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(m_thisWarrior);

	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

	bool hasOverlap = m_thisWarrior->GetWorld()->OverlapMultiByObjectType(
		overlaps,
		m_thisWarrior->GetActorLocation(),
		FQuat::Identity,
		objectQueryParams,
		FCollisionShape::MakeSphere(m_attackRange),
		queryParams
	);

	if (!hasOverlap || m_chargeCurrentDuration >= m_chargeDuration) return true;
	else return false;

}