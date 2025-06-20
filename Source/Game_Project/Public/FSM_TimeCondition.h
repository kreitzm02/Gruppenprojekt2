// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_TimeCondition.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_TimeCondition : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_minTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_maxTime = 5.0f;

	float m_timeUntilTrue = 0;
	float m_ellapsedTime = 0;
public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;
};
