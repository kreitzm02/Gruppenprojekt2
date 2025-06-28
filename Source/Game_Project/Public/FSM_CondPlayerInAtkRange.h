// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_CondPlayerInAtkRange.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_CondPlayerInAtkRange : public UFSM_BaseStateTransition
{
	GENERATED_BODY()

public:
	void Initialize() override;
	void ResetCondition() override;
	bool IsConditionMet(float a_deltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool m_enableDebug = true;

	UPROPERTY()
	AActor* m_owner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool m_checkForInRange = true;

	UPROPERTY()
	float m_attackRange;
};
