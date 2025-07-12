// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorChargeStart.h"

void UFSM_BossWarriorChargeStart::Initialize()
{
	Super::Initialize();
	m_thisWarrior = Cast<ABossEnemy_Warrior>(GetOuter()->GetOuter());
}

void UFSM_BossWarriorChargeStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossWarriorChargeStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisWarrior->GetChargeReady())
	{
		return true;
	}

	return false;
}