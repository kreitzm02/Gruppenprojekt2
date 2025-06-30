// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWidget_PlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetHealthPercent(float a_healthPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetStaminaPercent(float a_staminaPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityOneCooldownPercent(float a_cooldownPercent);
};
