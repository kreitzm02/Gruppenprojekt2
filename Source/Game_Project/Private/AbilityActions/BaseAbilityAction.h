// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbilityAction.generated.h"

UENUM(BlueprintType)
enum class EAAWeaponNames : uint8
{
	MESH_1H_AXE			UMETA(DisplayName = "1H_Axe"),
	MESH_1H_SWORD		UMETA(DisplayName = "1H_Sword"),
	MESH_1H_WAND		UMETA(DisplayName = "1H_Wand"),
	MESH_1H_CROSSBOW	UMETA(DisplayName = "1H_Crossbow"),
	MESH_DRUIDSTAFF		UMETA(DisplayName = "Druid_Staff"),
	MESH_1H_SPELLBOOK	UMETA(DisplayName = "Spellbook"),
	MESH_2H_STAFF		UMETA(DisplayName = "2H_Staff"),
	MESH_1H_DAGGER		UMETA(DisplayName = "1H_Dagger"),
	MESH_1H_SCYTHE		UMETA(DisplayName = "1H_Scythe"),
};

UENUM(BlueprintType)
enum class EAAAnimationNames : uint8
{
	ANIM_SPELLCAST_LONG		UMETA(DisplayName = "Spellcast_Long"),
	ANIM_SPELLCAST_RAISE	UMETA(DisplayName = "Spellcast_Raise"),
	ANIM_SPELLCAST_SHOOT	UMETA(DisplayName = "Spellcast_Shoot"),
	ANIM_1H_MELEE_ATTACK_CHOP			UMETA(DisplayName = "1H_Melee_Attack_Chop"),
	ANIM_1H_MELEE_ATTACK_SLICE_DIAGONAL	UMETA(DisplayName = "1H_Melee_Attack_Slice_Diagonal"),
	ANIM_1H_MELEE_ATTACK_STAB			UMETA(DisplayName = "1H_Melee_Attack_Stab")
};

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class UBaseAbilityAction : public UObject
{
	GENERATED_BODY()
	
public:

	virtual void PrepareAbilityAction(AActor* a_AbilityUser);
	virtual void PlayAbilityAction(AActor* a_AbilityUser);
	virtual void EndAbilityAction(AActor* a_AbilityUser);
	virtual FName GetWeaponMeshName(EAAWeaponNames a_WeaponMeshName) const;
};
