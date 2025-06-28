// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_CondPlayerOutAtkRange.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_CondPlayerOutAtkRange : public UFSM_BaseStateTransition
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

	float m_attackRange;

	float m_attackDuration = 0.0f;

	float m_passedTime = 0.0f;
};
