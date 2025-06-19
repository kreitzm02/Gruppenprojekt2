// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_ConditionPlayerInRange.generated.h"


/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_ConditionPlayerInRange : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AActor* owner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool checkForInRange = true;

	UPROPERTY()
	float detectionRadius;



public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;
};
