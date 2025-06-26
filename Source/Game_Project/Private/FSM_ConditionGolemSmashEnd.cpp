// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionGolemSmashEnd.h"

void UFSM_ConditionGolemSmashEnd::Initialize()
{
	Super::Initialize();
	m_thisGolem = Cast<AEnemy_Golem>(GetOuter()->GetOuter());
	m_smashDuration = m_thisGolem->GetSmashDuration();
}

void UFSM_ConditionGolemSmashEnd::ResetCondition()
{
	Super::ResetCondition();

	m_ellapsedTime = 0.0f;
}

bool UFSM_ConditionGolemSmashEnd::IsConditionMet(float a_deltaTime)
{
	m_ellapsedTime += a_deltaTime;

	if (m_ellapsedTime >= m_smashDuration)
	{
		return true;
	}

	return false;
}