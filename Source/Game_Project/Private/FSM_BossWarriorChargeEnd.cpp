// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorChargeEnd.h"

void UFSM_BossWarriorChargeEnd::Initialize()
{
	Super::Initialize();
	//owner = Cast<AActor>(GetOuter()->GetOuter());
	m_thisWarrior = Cast<ABossEnemy_Warrior>(GetOuter()->GetOuter());
	m_maxCharges = m_thisWarrior->GetMaxChargesPerAbility();
}

void UFSM_BossWarriorChargeEnd::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossWarriorChargeEnd::IsConditionMet(float a_deltaTime)
{
	if (m_thisWarrior->GetCurrentCharges() >= m_maxCharges)
	{
		return true;
	}
	return false;
}