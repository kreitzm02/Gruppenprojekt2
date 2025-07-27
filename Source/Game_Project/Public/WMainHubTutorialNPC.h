// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "WMainHubTutorialNPC.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWMainHubTutorialNPC : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Yes;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_No;

    UFUNCTION()
    void OnButton1Clicked();

    UFUNCTION()
    void OnButton2Clicked();
};
