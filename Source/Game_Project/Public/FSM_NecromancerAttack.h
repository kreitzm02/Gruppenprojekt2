// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Necromancer.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_NecromancerAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_NecromancerAttack : public UFSM_BaseEnemyState
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

	UPROPERTY()
	AEnemy_Necromancer* m_thisEnemy;

	UPROPERTY()
	AActor* m_player = nullptr;

	float m_passedTime = 0.0f;

	float m_animationDuration = 0.0f;

	bool m_animationStarted = false;
};
