// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossGolemSmashStart.h"

void UFSM_BossGolemSmashStart::Initialize()
{
	Super::Initialize();
	m_thisGolem = Cast<ABossEnemy_Golem>(GetOuter()->GetOuter());
}

void UFSM_BossGolemSmashStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_BossGolemSmashStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisGolem->GetSmashReady())
	{
		return true;
	}

	return false;
}