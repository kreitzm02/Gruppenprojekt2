// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossRogueArrowCirclesEnd.h"
#include "BossEnemy_Rogue.h"

void UFSM_BossRogueArrowCirclesEnd::Initialize()
{
	Super::Initialize();
	ABossEnemy_Rogue* thisRogue = Cast<ABossEnemy_Rogue>(GetOuter()->GetOuter());
	m_circleDuration = thisRogue->GetArrowSprayDuration();
}

void UFSM_BossRogueArrowCirclesEnd::ResetCondition()
{
	Super::ResetCondition();
	m_circleCurrentDuration = 0.0f;
}

bool UFSM_BossRogueArrowCirclesEnd::IsConditionMet(float a_deltaTime)
{
	m_circleCurrentDuration += a_deltaTime;

	if (m_circleCurrentDuration >= m_circleDuration) return true;
	else return false;
}