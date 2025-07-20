// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossRogueArrowCirclesStart.h"

void UFSM_BossRogueArrowCirclesStart::Initialize()
{
	Super::Initialize();
	m_thisRogue = Cast<ABossEnemy_Rogue>(GetOuter()->GetOuter());
}

void UFSM_BossRogueArrowCirclesStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossRogueArrowCirclesStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisRogue->GetArrowCircleReady())
	{
		return true;
	}

	return false;
}