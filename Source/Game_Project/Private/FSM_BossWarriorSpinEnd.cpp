// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorSpinEnd.h"

void UFSM_BossWarriorSpinEnd::Initialize()
{
	Super::Initialize();
	//owner = Cast<AActor>(GetOuter()->GetOuter());
	m_thisWarrior = Cast<ABossEnemy_Warrior>(GetOuter()->GetOuter());
	m_spinDuration = m_thisWarrior->GetSpinDuration();
}

void UFSM_BossWarriorSpinEnd::ResetCondition()
{
	Super::ResetCondition();
	m_spinCurrentDuration = 0.0f;
}

bool UFSM_BossWarriorSpinEnd::IsConditionMet(float a_deltaTime)
{
	m_spinCurrentDuration += a_deltaTime;

	if (m_spinCurrentDuration >= m_spinDuration) return true;
	else return false;

}