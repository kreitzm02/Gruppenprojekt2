// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Warrior.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_ConditionWarriorChargeStart.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_ConditionWarriorChargeStart : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

private:
	//UPROPERTY()
	//AActor* owner = nullptr;

	UPROPERTY()
	AEnemy_Warrior* thisWarrior = nullptr;

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;
};
