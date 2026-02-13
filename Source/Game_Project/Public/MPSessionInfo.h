// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MPSessionInfo.generated.h"

class UButton;
class UTextBlock;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UMPSessionInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;


private:
	FString m_lobbyCode = "CODE";

	virtual void NativeConstruct() override;

	UFUNCTION()
	void DisplayLobbyCode(const FString& a_lobbyCode);

	UFUNCTION()
	void CloseUI();

	UPROPERTY()
	UCanvasPanel* RootCanvas = nullptr;

	UPROPERTY()
	UTextBlock* TitleText = nullptr;

	UPROPERTY()
	UButton* ActionButton = nullptr;

	UPROPERTY()
	UTextBlock* ButtonLabel = nullptr;
};
