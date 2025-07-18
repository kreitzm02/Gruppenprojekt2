// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <NiagaraComponent.h>
#include "AbilityActions/BaseAbilityAction.h"
#include "StaminaAbilityAction.generated.h"

UENUM(BlueprintType)
enum class EStaminaWeaponNames : uint8
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
enum class EStaminaAnimationNames : uint8
{
	ANIM_SPELLCAST_LONG		UMETA(DisplayName = "Spellcast_Long"),
	ANIM_SPELLCAST_RAISE	UMETA(DisplayName = "Spellcast_Raise"),
	ANIM_SPELLCAST_SHOOT	UMETA(DisplayName = "Spellcast_Shoot")
};

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
	EStaminaWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Stamina Ability Action Settings", meta = (DisplayName = "Custom Weapon Mesh Name", EditCondition = "m_WeaponMeshName == EStaminaWeaponNames::MESH_CUSTOM", EditConditionHides))
	FName m_CustomWeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Stamina Ability Action Settings", meta = (DisplayName = "Animation Name"))
	EStaminaAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	FName GetWeaponMeshName() const;

private:

	FTimerHandle m_StartTimerHandle;
	FTimerHandle m_MoveTimerHandle;
	UAnimMontage* m_AttackMontage;
	UNiagaraComponent* m_VFXComp;
	void PlayStamina(AActor* a_AbilityUser);
	void MoveStamina(AActor* a_AbilityUser);
	void EndStamina(AActor* a_AbilityUser);
};
