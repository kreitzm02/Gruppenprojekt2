// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSave.h"
#include "Widget_GameTimer.h"
#include "Widget_PlayerUI.h"
#include "Engine/GameInstance.h"
#include "Game_GameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicVolumeChanged, float, a_newVolume);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSFXVolumeChanged, float, a_newVolume);

UCLASS()
class GAME_PROJECT_API UGame_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	void PrintStackInfo();

	UPROPERTY(BlueprintAssignable, Category = "Audio")
	FOnMusicVolumeChanged OnMusicVolumeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Audio")
	FOnMusicVolumeChanged OnSFXVolumeChanged;

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


	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float a_volume);
	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float a_volume);

	UFUNCTION(BlueprintCallable)
	float GetMusicVolume() { return m_musicVolume; }
	UFUNCTION(BlueprintCallable)
	float GetSFXVolume() { return m_sfxVolume; }

	void SetIsInLevel(bool a_isInLevel) { m_isInLevel = a_isInLevel; }

	UFUNCTION(BlueprintCallable)
	bool GetIsInLevel() { return m_isInLevel; }

	void SetPlayerUI(UWidget_PlayerUI* a_playerUI) { m_playerUI = a_playerUI; }

	UFUNCTION(BlueprintCallable)
	UWidget_PlayerUI* GetPlayerUI() { return m_playerUI; }

	void StartGameTimer();
	void StopTimer();

	bool TimerWidgetNotNullptr() { if (m_timerWidgetInstance) return true; return false; }

	void AddGameTimerToViewport();

	float m_AdditionalDamage = 0.0f;

	float m_AdditionalDefense = 0.0f;

	void SetLastOverworldPos(FVector a_lastPos) { m_lastPlayerOverworldPos = a_lastPos; }

	FVector GetLastOverworldPos() { return m_lastPlayerOverworldPos; }


	void ResetEnemyScaling() { m_enemyScaling = 1.0f; }

	void IncreaseEnemyScaling() { m_enemyScaling += m_enemyScalingIncreasePerDungeon; }

	float GetEnemyScaling() { return m_enemyScaling; }

private:
	void ShowInitialTutorial();

	void TickTimer();
	void OnTimerEnd();

	FTimerHandle m_gameTimerHandle;

	int m_remainingTime;

	UPROPERTY(EditAnywhere, Category = "Game Timer Settings")
	TSubclassOf<UUserWidget> m_timerWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "Game Timer Settings")
	int m_gameTimerDuration = 3000;

	UPROPERTY(EditAnywhere, Category = "Timer End Sound")
	USoundBase* m_timerFinishingSound;

	UPROPERTY()
	class UWidget_GameTimer* m_timerWidgetInstance = nullptr;

	UPROPERTY()
	UWidget_PlayerUI* m_playerUI = nullptr;

	float m_musicVolume = 0.5f;

	float m_sfxVolume = 0.5f;

	bool m_isInLevel = false;


	FVector m_lastPlayerOverworldPos = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Initial Tutorial")
	TSubclassOf<UUserWidget> m_initialTutorialUI;


	float m_enemyScaling = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Scaling")
	float m_enemyScalingIncreasePerDungeon = 0.2f;
};
