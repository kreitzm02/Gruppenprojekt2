// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossMageBurnGroundStart.h"

void UFSM_BossMageBurnGroundStart::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<ABossEnemy_Mage>(GetOuter()->GetOuter());
}

void UFSM_BossMageBurnGroundStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossMageBurnGroundStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisEnemy->GetBurnGroundReady())
	{
		return true;
	}

	return false;
}