// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_ConditionAlwaysFalse.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_ConditionAlwaysFalse : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;

};
