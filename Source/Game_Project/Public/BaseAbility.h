// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <AbilityActions/BaseAbilityAction.h>
#include "BaseAbility.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class GAME_PROJECT_API UBaseAbility : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText m_AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	UTexture2D* m_AbilityIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float m_CooldownTime = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool CanUse() const { return !m_IsOnCooldown; }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void UseAbility(AActor* a_AbilityUser);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void EquipAbility(AActor* a_AbilityUser);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetCooldownTime(UWorld* a_World);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ForceEndAbility(AActor* a_AbilityUser);

private:
	bool m_IsOnCooldown = false;

	bool m_IsInitialized = false;

	float m_ConstCooldownTime = 1.0f;

	void StartCooldown(UWorld* a_World);

	UPROPERTY(EditAnywhere, Instanced, Category = "Ability", meta = (DisplayName = "Enemy Abilities"))
	TArray<UBaseAbilityAction*> m_AbilityActions;

	FTimerHandle m_CooldownTimerHandle;
};
