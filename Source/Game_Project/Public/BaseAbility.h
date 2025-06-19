// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class GAME_PROJECT_API UBaseAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText m_AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float m_CooldownTime = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool CanUse() const { return !m_IsOnCooldown; }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void UseAbility(AActor* a_AbilityUser);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void EquipAbility(AActor* a_AbilityUser);

private:
	bool m_IsOnCooldown = false;

	void StartCooldown(UWorld* a_World);
};
