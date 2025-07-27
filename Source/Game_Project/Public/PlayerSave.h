// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include <AllAbilities.h>
#include "PlayerSave.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int CalcCostFromCurUpgrCount(int a_currentUpgradeCount) { return m_baseUpgradeCost + m_upgradeCostIncrease * a_currentUpgradeCount; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Saves")
	int m_currency = 0;

	UPROPERTY(EditAnywhere, Category = "Player Saves Settings")
	int m_baseUpgradeCost = 5;

	UPROPERTY(EditAnywhere, Category = "Player Saves Settings")
	int m_upgradeCostIncrease = 4;

	UFUNCTION(BlueprintCallable)
	float GetPlayerDmgMultiplier() { return m_damageMultiplier + m_damageMultiplierSteps * m_damageMultiplierCount; }

	UPROPERTY(VisibleAnywhere, Category = "Player Saves")
	float m_damageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Player Saves Settings")
	float m_damageMultiplierSteps = 0.01f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Saves")
	int m_damageMultiplierCount = 0;

	UFUNCTION(BlueprintCallable)
	float GetPlayerHPMultiplier() { return m_healthMultiplier + m_healthMultiplierSteps * m_healthMultiplierCount; }

	UPROPERTY(VisibleAnywhere, Category = "Player Saves")
	float m_healthMultiplier = 1.00f;

	UPROPERTY(EditAnywhere, Category = "Player Saves Settings")
	float m_healthMultiplierSteps = 0.01f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Saves")
	int m_healthMultiplierCount = 0;

	UFUNCTION(BlueprintCallable)
	float GetPlayerStaminaRegenMultiplier() { return m_staminaGainMultiplier + m_staminaGainMultiplierSteps * m_staminaGainMultiplierCount; }

	UPROPERTY(VisibleAnywhere, Category = "Player Saves")
	float m_staminaGainMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Player Saves Settings")
	float m_staminaGainMultiplierSteps = 0.01f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Saves")
	int m_staminaGainMultiplierCount = 0;

	UFUNCTION(BlueprintCallable)
	float GetPlayerXPGainMultiplier() { return m_xpGainMultiplier + m_xpGainMultiplierSteps * m_xpGainMultiplierCount; }

	UPROPERTY(VisibleAnywhere, Category = "Player Saves")
	float m_xpGainMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Player Saves Settings")
	float m_xpGainMultiplierSteps = 0.01f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Saves")
	int m_xpGainMultiplierCount = 0;

	UFUNCTION(BlueprintCallable)
	float GetPlayerHPRegenMultiplier() { return m_healthRegen + m_healthRegenSteps * m_healthRegenCount; }

	UPROPERTY(VisibleAnywhere, Category = "Player Saves")
	float m_healthRegen = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Player Saves Settings")
	float m_healthRegenSteps = 0.01f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Saves")
	int m_healthRegenCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Save")
	int m_CurrentPlayerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Save")
	TMap<EAllAbilities, int> m_AbilityLevels;

};
