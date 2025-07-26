// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSave.h"
#include "Widget_GameTimer.h"
#include "Engine/GameInstance.h"
#include "Game_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UGame_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly)
	UPlayerSave* m_playerSave = nullptr;

	void LoadOrCreateSaveGame();

	UFUNCTION(BlueprintCallable)
	bool TryBuyDmgBonus();

	UFUNCTION(BlueprintCallable)
	bool TryBuyHPBonus();

	UFUNCTION(BlueprintCallable)
	bool TryBuyStaminaRegen();

	UFUNCTION(BlueprintCallable)
	bool TryBuyXPBonus();

	UFUNCTION(BlueprintCallable)
	bool TryBuyHPRegen();





	void StartGameTimer();

private:
	void TickTimer();
	void OnTimerEnd();

	FTimerHandle m_gameTimerHandle;

	int m_remainingTime;

	UPROPERTY(EditAnywhere, Category = "Game Timer Settings")
	TSubclassOf<UUserWidget> m_timerWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "Game Timer Settings")
	int m_gameTimerDuration = 3000;

	UPROPERTY()
	class UWidget_GameTimer* m_timerWidgetInstance = nullptr;
};
