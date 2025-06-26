// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionRogueSprayEnd.h"

#include "Enemy_Rogue.h"

void UFSM_ConditionRogueSprayEnd::Initialize()
{
	Super::Initialize();
	AEnemy_Rogue* thisRogue = Cast<AEnemy_Rogue>(GetOuter()->GetOuter());
	m_sprayDuration = thisRogue->GetArrowSprayDuration();
}

void UFSM_ConditionRogueSprayEnd::ResetCondition()
{
	Super::ResetCondition();
	m_sprayCurrentDuration = 0.0f;
}

bool UFSM_ConditionRogueSprayEnd::IsConditionMet(float a_deltaTime)
{
	m_sprayCurrentDuration += a_deltaTime;

	if (m_sprayCurrentDuration >= m_sprayDuration) return true;
	else return false;
}