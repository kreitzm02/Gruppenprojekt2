// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include "DamageCircleAbilityAction.generated.h"

UENUM(BlueprintType)
enum class EDamageCircleWeaponNames : uint8
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
enum class EDamageCircleAnimationNames : uint8
{
	ANIM_SPELLCAST_LONG		UMETA(DisplayName = "Spellcast_Long"),
	ANIM_SPELLCAST_RAISE	UMETA(DisplayName = "Spellcast_Raise"),
	ANIM_SPELLCAST_SHOOT	UMETA(DisplayName = "Spellcast_Shoot")
};

UCLASS(Blueprintable, EditInlineNew)
class UDamageCircleAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Circle mesh"))
	UStaticMesh* m_DCMesh;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Damage Per Hit"))
	int32 m_DamagePerHit;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Time Between Hits"))
	float m_TimeBetweenHits;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Starting Delay"))
	float m_Delay;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Duration"))
	float m_Duration;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Circle Radius"))
	float m_CircleSize;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Circle Color"))
	FColor m_Color;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Circle Follows The User"))
	bool m_CircleFollowsUser;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EDamageCircleWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Custom Weapon Mesh Name", EditCondition = "m_WeaponMeshName == EDamageCircleWeaponNames::MESH_CUSTOM", EditConditionHides))
	FName m_CustomWeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Animation Name"))
	EDamageCircleAnimationNames m_AnimName;

	FName GetWeaponMeshName() const;
	void EndAbilityAction();
	void DrawDebug(AActor* a_AbilityUser);
	void MoveCircle(AActor* a_AbilityUser);

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;

private:

	AStaticMeshActor* m_DCMeshActor = nullptr;
	UAnimMontage* m_AttackMontage;
	FTimerHandle m_DamageTickTimerHandle;
	FTimerHandle m_DebugDrawTimerHandle;
	FTimerHandle m_CircleMoveTimerHandle;
	FTimerHandle m_EndTimerHandle;
	FTimerHandle m_StartTimerHandle;
	FVector m_StaticCircleLocation;
	TSet<AActor*> m_AlreadyHitActors;
	void PerformDamageTick(AActor* a_AbilityUser);
	void PlayDamageCircle(AActor* a_AbilityUser);
};
