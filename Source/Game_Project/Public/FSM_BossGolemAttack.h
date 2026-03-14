// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Golem.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossGolemAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossGolemAttack : public UFSM_BaseEnemyState
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
	ABossEnemy_Golem* m_thisEnemy;

	UPROPERTY()
	AActor* m_target = nullptr;

	float m_passedTime = 0.0f;

	float m_animationDuration = 0.0f;

	bool m_animationStarted = false;

	bool m_soundStarted = false;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_playSoundAtAnimOffset = 0.45f;
};
