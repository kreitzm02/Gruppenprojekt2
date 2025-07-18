// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossMageFireballStart.h"

void UFSM_BossMageFireballStart::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<ABossEnemy_Mage>(GetOuter()->GetOuter());
}

void UFSM_BossMageFireballStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossMageFireballStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisEnemy->GetFireballReady())
	{
		return true;
	}

	return false;
}