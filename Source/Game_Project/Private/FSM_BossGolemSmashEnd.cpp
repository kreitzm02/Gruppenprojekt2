// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemSmashEnd.h"

void UFSM_BossGolemSmashEnd::Initialize()
{
	Super::Initialize();

	m_thisGolem = Cast<ABossEnemy_Golem>(GetOuter()->GetOuter());
	m_maxSmashes = m_thisGolem->GetMaxSmashesPerAbility();
}

void UFSM_BossGolemSmashEnd::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossGolemSmashEnd::IsConditionMet(float a_deltaTime)
{
	if (m_thisGolem->GetCurrentSmashes() >= m_maxSmashes)
	{
		return true;
	}
	return false;
}