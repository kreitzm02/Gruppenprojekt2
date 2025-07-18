// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossMageFireballEnd.h"

void UFSM_BossMageFireballEnd::Initialize()
{
	Super::Initialize();
	m_thisMage = Cast<ABossEnemy_Mage>(GetOuter()->GetOuter());
	m_fireballDuration = m_thisMage->GetFireballCastTime();
}

void UFSM_BossMageFireballEnd::ResetCondition()
{
	Super::ResetCondition();

	m_ellapsedTime = 0.0f;
}

bool UFSM_BossMageFireballEnd::IsConditionMet(float a_deltaTime)
{
	m_ellapsedTime += a_deltaTime;

	if (m_ellapsedTime >= m_fireballDuration)
	{
		return true;
	}

	return false;
}