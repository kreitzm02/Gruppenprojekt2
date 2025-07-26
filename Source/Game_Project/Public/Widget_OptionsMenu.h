// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsMenu.generated.h"

class UWidget_PauseMenu;
/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWidget_OptionsMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UWidget_PauseMenu* m_pauseMenu = nullptr;

	UFUNCTION(BlueprintCallable)
	void BackToPauseMenu();

public:
	void SetPauseMenu(UWidget_PauseMenu* a_pauseMenu) { m_pauseMenu = a_pauseMenu; }
};
