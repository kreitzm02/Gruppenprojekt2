// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include <NiagaraComponent.h>
#include "SelfHealAbilityAction.generated.h"


UENUM(BlueprintType)
enum class ESelfHealWeaponNames : uint8
{
	MESH_1H_AXE			UMETA(DisplayName = "1H_Axe"),
	MESH_1H_SWORD		UMETA(DisplayName = "1H_Sword"),
	MESH_1H_WAND		UMETA(DisplayName = "1H_Wand"),
	MESH_1H_CROSSBOW	UMETA(DisplayName = "1H_Crossbow"),
	MESH_KNIFE			UMETA(DisplayName = "Knife"),
	MESH_2H_MACE		UMETA(DisplayName = "2H_Mace"),
	MESH_2H_STAFF		UMETA(DisplayName = "2H_Staff"),
	MESH_1H_DAGGER		UMETA(DisplayName = "1H_Dagger"),
	MESH_1H_SCYTHE		UMETA(DisplayName = "1H_Scythe"),
	MESH_CUSTOM			UMETA(DisplayName = "Custom - Use the FTEXT field. Must be the exact weapon name!")
};

UENUM(BlueprintType)
enum class ESelfHealAnimationNames : uint8
{
	ANIM_SPELLCAST_LONG		UMETA(DisplayName = "Spellcast_Long"),
	ANIM_SPELLCAST_RAISE	UMETA(DisplayName = "Spellcast_Raise"),
	ANIM_SPELLCAST_SHOOT	UMETA(DisplayName = "Spellcast_Shoot")
};

UCLASS()
class USelfHealAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "VFX"))
	UNiagaraSystem* m_HealVFX;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "HP to Heal"))
	float m_HPAmount;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "Delay"))
	float m_StartingDelay;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	ESelfHealWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "Custom Weapon Mesh Name", EditCondition = "m_WeaponMeshName == ESelfHealWeaponNames::MESH_CUSTOM", EditConditionHides))
	FName m_CustomWeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Heal Ability Action Settings", meta = (DisplayName = "Animation Name"))
	ESelfHealAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	FName GetWeaponMeshName() const;

private:

	FTimerHandle m_StartTimerHandle;
	FTimerHandle m_MoveTimerHandle;
	UAnimMontage* m_AttackMontage;
	UNiagaraComponent* m_VFXComp;
	void PlaySelfHeal(AActor* a_AbilityUser);
	void MoveSelfHeal(AActor* a_AbilityUser);
	void EndSelfHeal(AActor* a_AbilityUser);
};
