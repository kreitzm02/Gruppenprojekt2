// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionNecroSummonEnd.h"

void UFSM_ConditionNecroSummonEnd::Initialize()
{
	Super::Initialize();
}

void UFSM_ConditionNecroSummonEnd::ResetCondition()
{
	Super::ResetCondition();;
	m_ellapsedTime = 0;
}

bool UFSM_ConditionNecroSummonEnd::IsConditionMet(float a_deltaTime)
{
	m_ellapsedTime += a_deltaTime;
	return m_ellapsedTime >= m_summonDuration;
}