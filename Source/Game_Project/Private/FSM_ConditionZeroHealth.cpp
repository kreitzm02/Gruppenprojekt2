// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionZeroHealth.h"
#include "EnemyCharacter.h"

void UFSM_ConditionZeroHealth::Initialize()
{
	owner = Cast<AActor>(GetOuter()->GetOuter());

	enemy = Cast<AEnemyCharacter>(owner);
}

void UFSM_ConditionZeroHealth::ResetCondition()
{

}

bool UFSM_ConditionZeroHealth::IsConditionMet(float a_deltaTime)
{
	return enemy->GetCurrentHealth() <= 0.0f;
	
}