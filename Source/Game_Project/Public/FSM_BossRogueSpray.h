// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Rogue.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossRogueSpray.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossRogueSpray : public UFSM_BaseEnemyState
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
	ABossEnemy_Rogue* m_thisEnemy;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shootAtAnimStartOffset = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_slowerMultiplier = 0.5f;

	UPROPERTY()
	AActor* m_target = nullptr;

	bool m_shotFired = false;

	float m_multiplier;

	float m_animDuration;

	float m_passedTime;
};
