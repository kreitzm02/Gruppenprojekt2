// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Golem.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossGolemJump.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossGolemJump : public UFSM_BaseEnemyState
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltatime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_jumpAnimation;

	UPROPERTY()
	ACharacter* m_target = nullptr;

	UPROPERTY()
	ABossEnemy_Golem* m_owner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_jumpHeight = 500.0f;

	float m_detectionRange = 0.0f;

	float m_animationLength = 0.0f;

	float m_ellapsedTime = 0.0f;

	bool m_isJumping = false;

	float m_currentMultiplier = 1.0f;

	FVector m_jumpToLocation = FVector::ZeroVector;

	FVector m_jumpStartLocation = FVector::ZeroVector;
};
