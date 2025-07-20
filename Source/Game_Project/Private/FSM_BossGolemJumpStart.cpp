// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemJumpStart.h"

void UFSM_BossGolemJumpStart::Initialize()
{
	Super::Initialize();
	m_thisGolem = Cast<ABossEnemy_Golem>(GetOuter()->GetOuter());
}

void UFSM_BossGolemJumpStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossGolemJumpStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisGolem->GetJumpReady())
	{
		return true;
	}

	return false;
}