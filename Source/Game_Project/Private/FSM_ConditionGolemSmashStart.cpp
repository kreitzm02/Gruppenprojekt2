// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionGolemSmashStart.h"

void UFSM_ConditionGolemSmashStart::Initialize()
{
	Super::Initialize();
	m_thisGolem = Cast<AEnemy_Golem>(GetOuter()->GetOuter());
}

void UFSM_ConditionGolemSmashStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_ConditionGolemSmashStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisGolem->GetSmashReady())
	{
		return true;
	}

	return false;
}