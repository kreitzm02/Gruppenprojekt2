// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionRogueSprayStart.h"

void UFSM_ConditionRogueSprayStart::Initialize()
{
	Super::Initialize();
	m_thisRogue = Cast<AEnemy_Rogue>(GetOuter()->GetOuter());
}

void UFSM_ConditionRogueSprayStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_ConditionRogueSprayStart::IsConditionMet(float a_deltaTime)
{
	if (m_thisRogue->GetArrowSprayReady())
	{
		return true;
	}
	return false;
}

