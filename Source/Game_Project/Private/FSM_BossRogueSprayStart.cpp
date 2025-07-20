// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossRogueSprayStart.h"

void UFSM_BossRogueSprayStart::Initialize()
{
	Super::Initialize();
	m_thisRogue = Cast<ABossEnemy_Rogue>(GetOuter()->GetOuter());
}

void UFSM_BossRogueSprayStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossRogueSprayStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisRogue->GetArrowSprayReady())
	{
		return true;
	}

	return false;
}