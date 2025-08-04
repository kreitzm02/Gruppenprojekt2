// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <NiagaraComponent.h>
#include "AbilityActions/BaseAbilityAction.h"
#include "StaminaAbilityAction.generated.h"

UCLASS()
class GAME_PROJECT_API UStaminaAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Stamina Ability Action Settings", meta = (DisplayName = "VFX"))
	UNiagaraSystem* m_StaminaVFX;

	UPROPERTY(EditAnywhere, Category = "Stamina Ability Action Settings", meta = (DisplayName = "HP to Heal"))
	float m_StaminaAmount;

	UPROPERTY(EditAnywhere, Category = "Stamina Ability Action Settings", meta = (DisplayName = "Delay"))
	float m_StartingDelay;

	UPROPERTY(EditAnywhere, Category = "Stamina Ability Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EAAWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Stamina Ability Action Settings", meta = (DisplayName = "Animation Name"))
	EAAAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	virtual void EndAbilityAction(AActor* a_AbilityUser) override;

private:
	UPROPERTY()
	FTimerHandle m_StartTimerHandle;

	UPROPERTY()
	FTimerHandle m_MoveTimerHandle;

	UPROPERTY()
	UAnimMontage* m_AttackMontage;

	UPROPERTY()
	UNiagaraComponent* m_VFXComp;

	UFUNCTION()
	void PlayStamina(AActor* a_AbilityUser);

	UFUNCTION()
	void MoveStamina(AActor* a_AbilityUser);
};
