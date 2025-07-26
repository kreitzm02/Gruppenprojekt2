// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_GameTimer.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWidget_GameTimer : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void UpdateGameTime(int a_remainingGameTime);
};
