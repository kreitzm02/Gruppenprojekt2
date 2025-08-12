// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AllAbilities.h"
#include "BaseAbility.h"
#include "MainAbilityContainerDataAsset.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class GAME_PROJECT_API UMainAbilityContainerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "Ability Type"))
	EAllAbilities m_ThisAbility;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (DisplayName = "Level 1 Version"))
	TSubclassOf<UBaseAbility> m_LVL1;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (DisplayName = "Level 2 Version"))
	TSubclassOf<UBaseAbility> m_LVL2;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (DisplayName = "Level 3 Version"))
	TSubclassOf<UBaseAbility> m_LVL3;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "General Display Name"))
	FString m_DisplayName;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "Description", MultiLine = "true"))
	FText m_Description;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "General Display Icon"))
	UTexture2D* m_Icon;

	EAllAbilities GetAbilityType() { return m_ThisAbility; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UBaseAbility> GetAbility(int a_Lvl);
};
