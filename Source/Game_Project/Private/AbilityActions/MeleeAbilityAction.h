// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include "MeleeAbilityAction.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class UMeleeAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Additional Damage"))
	int32 m_AdditionalDamage;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Sound Effect"))
	USoundWave* m_SoundEffect;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Allow Critical Hits"))
	bool m_AllowCriticalHits;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EAAWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Animation Name"))
	EAAAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	virtual void EndAbilityAction(AActor* a_AbilityUser) override;
	void SetMeleeDamage(AActor* a_AbilityUser);
	void DestroyMeleeVFX(AActor* a_AbilityUser);
private:
	UPROPERTY()
	UAnimMontage* m_AttackMontage;

	FTimerHandle m_EndTimerHandle;
};


