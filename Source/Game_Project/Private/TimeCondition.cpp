// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeCondition.h"

void UTimeCondition::Initialize()
{
	timeUntilTrue = FMath::RandRange(minTime,maxTime);
	ellapsedTime = 0;
}

bool UTimeCondition::IsConditionMet()
{
	ellapsedTime += GetWorld()->GetDeltaSeconds();
	return ellapsedTime >= timeUntilTrue;
}
