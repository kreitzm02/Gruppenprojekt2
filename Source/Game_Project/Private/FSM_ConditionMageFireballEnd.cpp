// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionMageFireballEnd.h"

void UFSM_ConditionMageFireballEnd::Initialize()
{
	Super::Initialize();
	m_thisMage = Cast<AEnemy_Mage>(GetOuter()->GetOuter());
	m_fireballDuration = m_thisMage->GetFireballCastTime();
}

void UFSM_ConditionMageFireballEnd::ResetCondition()
{
	Super::ResetCondition();

	m_ellapsedTime = 0.0f;
}

bool UFSM_ConditionMageFireballEnd::IsConditionMet(float a_deltaTime)
{
	m_ellapsedTime += a_deltaTime;

	if (m_ellapsedTime >= m_fireballDuration)
	{
		return true;
	}

	return false;
}