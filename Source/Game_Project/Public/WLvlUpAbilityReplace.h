// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include "WLvlUpAbilityReplace.generated.h"


/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UWLvlUpAbilityReplace : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Ability1;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Ability2;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Ability3;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Ability4;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Skip;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Ability1;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Ability2;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Ability3;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Ability4;

    UPROPERTY(meta = (BindWidget))
    UImage* Image_Ability1;

    UPROPERTY(meta = (BindWidget))
    UImage* Image_Ability2;

    UPROPERTY(meta = (BindWidget))
    UImage* Image_Ability3;

    UPROPERTY(meta = (BindWidget))
    UImage* Image_Ability4;

    void SetButtonTexts(const FText& a_T1, const FText& a_T2, const FText& a_T3, const FText& a_T4);

    void SetButtonImages(UTexture2D* a_I1, UTexture2D* a_I2, UTexture2D* a_I3, UTexture2D* a_I4);

    UFUNCTION()
    void OnButton1Clicked();

    UFUNCTION()
    void OnButton2Clicked();

    UFUNCTION()
    void OnButton3Clicked();

    UFUNCTION()
    void OnButton4Clicked();

    UFUNCTION()
    void OnButton5Clicked();
};
