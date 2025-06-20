// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionZeroHealth.h"
#include "EnemyCharacter.h"

void UFSM_ConditionZeroHealth::Initialize()
{
	m_owner = Cast<AActor>(GetOuter()->GetOuter());

	m_enemy = Cast<AEnemyCharacter>(m_owner);
}

void UFSM_ConditionZeroHealth::ResetCondition()
{

}

bool UFSM_ConditionZeroHealth::IsConditionMet(float a_deltaTime)
{
	return m_enemy->GetCurrentHealth() <= 0.0f;
	
}