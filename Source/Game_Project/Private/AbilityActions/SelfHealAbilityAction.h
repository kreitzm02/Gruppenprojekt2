// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include <NiagaraComponent.h>
#include "SelfHealAbilityAction.generated.h"


UCLASS()
class USelfHealAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "VFX"))
	UNiagaraSystem* m_HealVFX;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "HP to Heal (in %)"))
	float m_HPAmount;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "Delay"))
	float m_StartingDelay;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EAAWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "Animation Name"))
	EAAAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	virtual void EndAbilityAction(AActor* a_AbilityUser) override;

private:

	FTimerHandle m_StartTimerHandle;
	FTimerHandle m_MoveTimerHandle;
	UAnimMontage* m_AttackMontage;
	UNiagaraComponent* m_VFXComp;
	void PlaySelfHeal(AActor* a_AbilityUser);
	void MoveSelfHeal(AActor* a_AbilityUser);
};
