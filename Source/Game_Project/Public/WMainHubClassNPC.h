// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "WMainHubClassNPC.generated.h"


/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWMainHubClassNPC : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Class1;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Class2;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Class3;

    UFUNCTION()
    void OnButton1Clicked();

    UFUNCTION()
    void OnButton2Clicked();

    UFUNCTION()
    void OnButton3Clicked();
};
