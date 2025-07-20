// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossRogueSprayEnd.h"
#include "BossEnemy_Rogue.h"

void UFSM_BossRogueSprayEnd::Initialize()
{
	Super::Initialize();
	ABossEnemy_Rogue* thisRogue = Cast<ABossEnemy_Rogue>(GetOuter()->GetOuter());
	m_sprayDuration = thisRogue->GetArrowSprayDuration();
}

void UFSM_BossRogueSprayEnd::ResetCondition()
{
	Super::ResetCondition();
	m_sprayCurrentDuration = 0.0f;
}

bool UFSM_BossRogueSprayEnd::IsConditionMet(float a_deltaTime)
{
	m_sprayCurrentDuration += a_deltaTime;

	if (m_sprayCurrentDuration >= m_sprayDuration) return true;
	else return false;
}