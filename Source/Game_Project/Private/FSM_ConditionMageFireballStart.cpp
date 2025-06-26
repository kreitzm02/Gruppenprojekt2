// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionMageFireballStart.h"

void UFSM_ConditionMageFireballStart::Initialize()
{
	Super::Initialize();
	m_thisMage = Cast<AEnemy_Mage>(GetOuter()->GetOuter());
}

void UFSM_ConditionMageFireballStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_ConditionMageFireballStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisMage->GetAbilityReady())
	{
		return true;
	}

	return false;
}