// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyState.h"
#include "IdleWalkAround.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class GAME_PROJECT_API UIdleWalkAround : public UBaseEnemyState
{
	GENERATED_BODY()
public:
	virtual void OnEnter() override;
	
	virtual void OnUpdate(float a_deltatime) override;
	
	virtual void OnExit() override;

private:
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence* walkAnimation;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float walkSpeed = 100.0f;

	FVector targetLocation;
	float moveDuration;
	float elapsedTime;

	void ChooseRandomNavLocation(AActor* owner);
};
