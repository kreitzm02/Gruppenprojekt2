// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <NiagaraComponent.h>
#include "AbilityActions/BaseAbilityAction.h"
#include "ShieldAbilityAction.generated.h"

/**
 * 
 */
UCLASS()
class UShieldAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Shield Ability Action Settings", meta = (DisplayName = "VFX"))
	UNiagaraSystem* m_ShieldVFX;

	UPROPERTY(EditAnywhere, Category = "Shield Ability Action Settings", meta = (DisplayName = "Defense to Add"))
	float m_DPAmount;

	UPROPERTY(EditAnywhere, Category = "Shield Ability Action Settings", meta = (DisplayName = "Duration"))
	float m_Duration;

	UPROPERTY(EditAnywhere, Category = "Shield Ability Action Settings", meta = (DisplayName = "Delay"))
	float m_StartingDelay;

	UPROPERTY(EditAnywhere, Category = "Shield Ability Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EAAWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Shield Ability Action Settings", meta = (DisplayName = "Animation Name"))
	EAAAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	virtual void EndAbilityAction(AActor* a_AbilityUser) override;

private:

	FTimerHandle m_StartTimerHandle;
	FTimerHandle m_MoveTimerHandle;
	FTimerHandle m_EndTimerHandle;
	UAnimMontage* m_AttackMontage;
	UNiagaraComponent* m_VFXComp;
	float m_SavedPlayerDP;
	void PlayShield(AActor* a_AbilityUser);
	void MoveShield(AActor* a_AbilityUser);
};
