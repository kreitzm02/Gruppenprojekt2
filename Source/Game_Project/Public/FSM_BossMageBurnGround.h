// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Mage.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossMageBurnGround.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossMageBurnGround : public UFSM_BaseEnemyState
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltaTime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_castBurnGroundAnimation = nullptr;

	UPROPERTY()
	ACharacter* m_target = nullptr;

	UPROPERTY()
	ABossEnemy_Mage* m_owner;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_burnGroundAtAnimStartOffset = 0.0f;

	float m_animationLength = 0.0f;

	float m_detectionRange = 0.0f;

	float m_currentMultiplier = 1.0f;

	bool m_burnGroundFired = false;

	float m_passedTime = 0.0f;
};
