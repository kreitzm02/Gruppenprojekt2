// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_NecroSummon.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_RiseFromGround.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_RiseFromGround : public UFSM_BaseEnemyState
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltatime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_riseAnimation;

	UPROPERTY()
	AEnemy_NecroSummon* m_thisEnemy;

	FVector startVec = FVector::ZeroVector;

	FVector endVec = FVector::ZeroVector;

	float m_duration = 0.0f;

	float m_passedTime = 0.0f;
};
