// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbility.h"
#include "AllAbilities.h"
#include "MainAbilityContainer.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UMainAbilityContainer : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "General Display Name"))
	FString m_DisplayName;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "General Display Icon"))
	UTexture2D* m_Icon;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "Ability Type"))
	EAllAbilities m_ThisAbility;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (DisplayName = "Level 1 Version"))
	TSubclassOf<UBaseAbility> m_LVL1;
	
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (DisplayName = "Level 2 Version"))
	TSubclassOf<UBaseAbility> m_LVL2;
	
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (DisplayName = "Level 3 Version"))
	TSubclassOf<UBaseAbility> m_LVL3;

public:
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIcon() { return m_Icon; }
	UFUNCTION(BlueprintCallable)
	FString GetDisplayName() { return m_DisplayName; }
	UFUNCTION(BlueprintCallable)
	EAllAbilities GetAbilityType() { return m_ThisAbility; }
};
