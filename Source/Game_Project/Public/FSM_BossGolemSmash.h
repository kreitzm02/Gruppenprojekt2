// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Golem.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossGolemSmash.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossGolemSmash : public UFSM_BaseEnemyState
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
	ACharacter* m_player = nullptr;

	UPROPERTY()
	ABossEnemy_Golem* m_owner;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shockwaveAtAnimStartOffset = 0.0f;

	float m_animationLength = 0.0f;

	float m_detectionRange = 0.0f;

	float m_currentMultiplier = 1.0f;

	bool m_shockwaveStarted = false;

	float m_passedTime = 0.0f;
};
