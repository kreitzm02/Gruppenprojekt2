// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_ChasePlayer : public UFSM_BaseEnemyState
{
	GENERATED_BODY()
	
public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltatime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_chaseAnimation;

	UPROPERTY()
	float m_walkSpeed = 100.0f;

	UPROPERTY()
	float m_chaseRange = 0.0f;

	UPROPERTY()
	ACharacter* m_target;
	
	FVector m_targetLocation;
};
