// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_Attack.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_Attack : public UFSM_BaseEnemyState
{
	GENERATED_BODY()
	
public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltaTime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_attackAnimation = nullptr;
};
