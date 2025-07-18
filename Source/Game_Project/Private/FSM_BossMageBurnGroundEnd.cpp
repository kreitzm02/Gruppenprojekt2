// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossMageBurnGroundEnd.h"

void UFSM_BossMageBurnGroundEnd::Initialize()
{
	Super::Initialize();
	//owner = Cast<AActor>(GetOuter()->GetOuter());
	m_thisEnemy = Cast<ABossEnemy_Mage>(GetOuter()->GetOuter());
	m_maxBurnGounds = m_thisEnemy->GetMaxBurnGroundsPerAbility();
}

void UFSM_BossMageBurnGroundEnd::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossMageBurnGroundEnd::IsConditionMet(float a_deltaTime)
{
	if (m_thisEnemy->GetCurrentBurnGround() >= m_maxBurnGounds)
	{
		return true;
	}
	return false;
}