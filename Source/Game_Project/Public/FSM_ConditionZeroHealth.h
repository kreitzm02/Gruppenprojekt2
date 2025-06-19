// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "EnemyCharacter.h"
#include "FSM_ConditionZeroHealth.generated.h"


/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_ConditionZeroHealth : public UFSM_BaseStateTransition
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	AActor* owner = nullptr;

	UPROPERTY()
	AEnemyCharacter* enemy = nullptr;

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;
};
