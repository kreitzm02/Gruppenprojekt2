// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionNecroSummonStart.h"

void UFSM_ConditionNecroSummonStart::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_Necromancer>(GetOuter()->GetOuter());
	m_maxSummons = m_thisEnemy->GetMaxSummons();
}

void UFSM_ConditionNecroSummonStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_ConditionNecroSummonStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisEnemy->GetSummonReady() && m_thisEnemy->GetCurrentSummons() < m_maxSummons)
	{
		return true;
	}

	return false;
}