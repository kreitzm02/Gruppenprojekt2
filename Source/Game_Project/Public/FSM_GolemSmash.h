// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Golem.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_GolemSmash.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_GolemSmash : public UFSM_BaseEnemyState
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltaTime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_smashAnimation = nullptr;

	UPROPERTY()
	AEnemy_Golem* m_thisEnemy;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shockwaveAtAnimStartOffset = 0.0f;

	bool m_shockwaveStarted = false;

	float m_passedTime = 0.0f;
};
