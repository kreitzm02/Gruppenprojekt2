// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemJumpEnd.h"

void UFSM_BossGolemJumpEnd::Initialize()
{
	Super::Initialize();
	//owner = Cast<AActor>(GetOuter()->GetOuter());
	m_thisGolem = Cast<ABossEnemy_Golem>(GetOuter()->GetOuter());
	m_maxJumps = m_thisGolem->GetMaxJumpsPerAbility();
}

void UFSM_BossGolemJumpEnd::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossGolemJumpEnd::IsConditionMet(float a_deltaTime)
{
	if (m_thisGolem->GetCurrentJumps() >= m_maxJumps)
	{
		return true;
	}
	return false;
}