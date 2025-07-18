// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_BossRogueArrowCirclesEnd.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossRogueArrowCirclesEnd : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;

private:
	float m_circleDuration;

	float m_circleCurrentDuration;
};
