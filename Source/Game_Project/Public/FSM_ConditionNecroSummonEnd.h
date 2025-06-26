// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_ConditionNecroSummonEnd.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_ConditionNecroSummonEnd : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_summonDuration = 0.0f;

	float m_ellapsedTime = 0.0f;
};
