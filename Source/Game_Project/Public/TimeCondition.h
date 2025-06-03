// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStateTransition.h"
#include "TimeCondition.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UTimeCondition : public UBaseStateTransition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	float minTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float maxTime = 5.0f;

	float timeUntilTrue = 0;
	float ellapsedTime = 0;
public:
	void Initialize() override;
	bool IsConditionMet() override;
};
