// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Warrior.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_BossWarriorSpinEnd.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossWarriorSpinEnd : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;

private:
	UPROPERTY()
	ABossEnemy_Warrior* m_thisWarrior = nullptr;

	float m_spinDuration;

	float m_spinCurrentDuration;
};
