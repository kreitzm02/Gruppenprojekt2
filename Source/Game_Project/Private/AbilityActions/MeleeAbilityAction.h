// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include "MeleeAbilityAction.generated.h"

UENUM(BlueprintType)
enum class EWeaponNames : uint8
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
enum class EAnimationNames : uint8
{
	ANIM_1H_MELEE_ATTACK_CHOP			UMETA(DisplayName = "1H_Melee_Attack_Chop"),
	ANIM_1H_MELEE_ATTACK_SLICE_DIAGONAL	UMETA(DisplayName = "1H_Melee_Attack_Slice_Diagonal"),
	ANIM_1H_MELEE_ATTACK_STAB			UMETA(DisplayName = "1H_Melee_Attack_Stab")
};

UCLASS(Blueprintable, EditInlineNew)
class UMeleeAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Additional Damage"))
	int32 m_AdditionalDamage;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Allow Critical Hits"))
	bool m_AllowCriticalHits;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Custom Weapon Mesh Name", EditCondition = "m_WeaponMeshName == EWeaponNames::MESH_CUSTOM", EditConditionHides))
	FName m_CustomWeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Melee Ability Action Settings", meta = (DisplayName = "Animation Name"))
	EAnimationNames m_AnimName;

	FName GetWeaponMeshName() const;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;

private:

	UAnimMontage* m_AttackMontage;
};


