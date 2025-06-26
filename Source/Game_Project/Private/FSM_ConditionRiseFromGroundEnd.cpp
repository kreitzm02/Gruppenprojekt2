// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionRiseFromGroundEnd.h"

void UFSM_ConditionRiseFromGroundEnd::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_NecroSummon>(GetOuter()->GetOuter());
	m_riseDuration = m_thisEnemy->GetRiseDuration();
}

void UFSM_ConditionRiseFromGroundEnd::ResetCondition()
{
	Super::ResetCondition();

	m_ellapsedTime = 0.0f;
}

bool UFSM_ConditionRiseFromGroundEnd::IsConditionMet(float a_deltaTime)
{
	m_ellapsedTime += a_deltaTime;

	if (m_ellapsedTime >= m_riseDuration)
	{
		return true;
	}

	return false;
}