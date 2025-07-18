// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include <NiagaraSystem.h>
#include "NiagaraComponent.h"
#include "TornadoAbilityAction.generated.h"

UENUM(BlueprintType)
enum class ETornadoWeaponNames : uint8
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
enum class ETornadoAnimationNames : uint8
{
	ANIM_SPELLCAST_LONG		UMETA(DisplayName = "Spellcast_Long"),
	ANIM_SPELLCAST_RAISE	UMETA(DisplayName = "Spellcast_Raise"),
	ANIM_SPELLCAST_SHOOT	UMETA(DisplayName = "Spellcast_Shoot")
};

struct FTornadoInstance
{
	FVector m_Direction;
	FVector m_CurrPosition;
	TSet<AActor*> m_AlreadyHitActors;
	UNiagaraComponent* m_VFXComp = nullptr;
	FTimerHandle m_MoveHandle;
	FTimerHandle m_EndHandle;
};

UCLASS()
class UTornadoAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Niagara VFX", meta = (DisplayName = "Tornado vfx"))
	UNiagaraSystem* m_TornadoVFX;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Spawn Radius"))
	float m_SpawnRadius = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Speed"))
	float m_Speed = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Randomness"))
	float m_Randomness = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Duration"))
	float m_Duration = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Collision Radius"))
	float m_CollisionRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Delay"))
	float m_Delay = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Weapon Mesh Name"))
	ETornadoWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Custom Weapon Mesh Name", EditCondition = "m_WeaponMeshName == ETornadoWeaponNames::MESH_CUSTOM", EditConditionHides))
	FName m_CustomWeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Animation Name"))
	ETornadoAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	FName GetWeaponMeshName() const;

private:

	void PlayTornado(AActor* a_AbilityUser);
	void MoveTornadoTick(TSharedPtr<FTornadoInstance> a_Instance);
	void EndTornado(TSharedPtr<FTornadoInstance> a_Instance);
	UAnimMontage* m_AttackMontage;
	FTimerHandle m_StartTimerHandle;
 };
