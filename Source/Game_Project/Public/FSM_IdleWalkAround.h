// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_IdleWalkAround.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class GAME_PROJECT_API UFSM_IdleWalkAround : public UFSM_BaseEnemyState
{
	GENERATED_BODY()
public:
	virtual void Initialize() override;

	virtual void OnEnter() override;
	
	virtual void OnUpdate(float a_deltatime) override;
	
	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_walkAnimation;

	UPROPERTY()
	float m_walkSpeed = 100.0f;

	FVector m_targetLocation;

	void ChooseRandomNavLocation(AActor* owner);
};
