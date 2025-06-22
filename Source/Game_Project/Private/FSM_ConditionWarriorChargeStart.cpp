// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionWarriorChargeStart.h"

#include "Enemy_Warrior.h"

void UFSM_ConditionWarriorChargeStart::Initialize()
{
	Super::Initialize();
	//owner = Cast<AActor>(GetOuter()->GetOuter());
	m_thisWarrior = Cast<AEnemy_Warrior>(GetOuter()->GetOuter());
}

void UFSM_ConditionWarriorChargeStart::ResetCondition()
{
	Super::ResetCondition();
}

bool UFSM_ConditionWarriorChargeStart::IsConditionMet(float a_deltaTime)
{

	if (m_thisWarrior->GetChargeReady())
	{
		return true;
	}

	return false;

}