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
	void SetExpPercent(float a_expPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityOneCooldownPercent(float a_cooldownPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityOneIcon(UTexture2D* a_abilityIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityTwoCooldownPercent(float a_cooldownPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityTwoIcon(UTexture2D* a_abilityIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityThreeCooldownPercent(float a_cooldownPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityThreeIcon(UTexture2D* a_abilityIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityFourCooldownPercent(float a_cooldownPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void SetAbilityFourIcon(UTexture2D* a_abilityIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void UpdateShownMoney();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI")
	void ShowMoneyInPlayerUI(bool a_shouldShow);
};
