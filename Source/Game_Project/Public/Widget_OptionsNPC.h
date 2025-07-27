// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/Button.h>
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsNPC.generated.h"


/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWidget_OptionsNPC : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Back;

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();
};
