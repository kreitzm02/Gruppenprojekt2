// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_TimeCondition.h"
#include "GameFramework/Character.h"

void UFSM_TimeCondition::Initialize()
{
	
}

void UFSM_TimeCondition::ResetCondition()
{
	timeUntilTrue = FMath::RandRange(minTime,maxTime);
	ellapsedTime = 0;
}

bool UFSM_TimeCondition::IsConditionMet(float a_deltaTime)
{
	ellapsedTime += GetWorld()->GetDeltaSeconds();
	return ellapsedTime >= timeUntilTrue;
}
