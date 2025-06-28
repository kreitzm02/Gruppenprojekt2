// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include "MinionAbilityAction.generated.h"

UENUM(BlueprintType)
enum class EMinionWeaponNames : uint8
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
enum class EMinionAnimationNames : uint8
{
	ANIM_SPELLCAST_LONG		UMETA(DisplayName = "Spellcast_Long"),
	ANIM_SPELLCAST_RAISE	UMETA(DisplayName = "Spellcast_Raise"),
	ANIM_SPELLCAST_SHOOT	UMETA(DisplayName = "Spellcast_Shoot")
};

UCLASS(Blueprintable, EditInlineNew)
class UMinionAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
};
