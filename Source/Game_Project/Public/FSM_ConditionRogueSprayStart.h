// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Rogue.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_ConditionRogueSprayStart.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_ConditionRogueSprayStart : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;

private:
	UPROPERTY()
	AEnemy_Rogue* m_thisRogue = nullptr;
};
