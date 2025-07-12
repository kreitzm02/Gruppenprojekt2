// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorSpinStart.h"

void UFSM_BossWarriorSpinStart::Initialize()
{
	Super::Initialize();
	m_thisWarrior = Cast<ABossEnemy_Warrior>(GetOuter()->GetOuter());
}

void UFSM_BossWarriorSpinStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossWarriorSpinStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisWarrior->GetSpinReady())
	{
		return true;
	}

	return false;
}