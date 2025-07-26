// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_PauseMenu.generated.h"

class UWidget_OptionsMenu;
/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWidget_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void ResumeGame();

	UFUNCTION(BlueprintCallable)
	void OpenOptionsWidget();

	UFUNCTION(BlueprintCallable)
	void EndCurrentRound();

	UFUNCTION(BlueprintCallable)
	void QuitGame();

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<UUserWidget> m_widgetOptionsClass = nullptr;

	UPROPERTY()
	UWidget_OptionsMenu* m_optionsMenu = nullptr;
};
