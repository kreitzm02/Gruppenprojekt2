// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyState.h"
#include "IdleStand.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UIdleStand : public UBaseEnemyState
{
	GENERATED_BODY()
public:
	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltatime) override;

	virtual void OnExit() override;

private:
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence* idleAnimation;
};
