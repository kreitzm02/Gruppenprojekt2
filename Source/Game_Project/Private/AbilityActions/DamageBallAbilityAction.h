// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include "DamageBallAbilityAction.generated.h"

UENUM(BlueprintType)
enum class EDamageBallWeaponNames : uint8
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
enum class EDamageBallAnimationNames : uint8
{
	ANIM_SPELLCAST_LONG		UMETA(DisplayName = "Spellcast_Long"),
	ANIM_SPELLCAST_RAISE	UMETA(DisplayName = "Spellcast_Raise"),
	ANIM_SPELLCAST_SHOOT	UMETA(DisplayName = "Spellcast_Shoot")
};

struct FDamageBallInstance
{
	FVector m_Direction;
	FVector m_CurrPosition;
	int32 m_HitCount = 0;
	TSet<AActor*> m_AlreadyHitActors;
	float m_CircleAngle;
};

UCLASS(Blueprintable, EditInlineNew)
class UDamageBallAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Damage"))
	int32 m_Damage;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Collisions Before Destruction"))
	int32 m_CollisionsBeforeDestruction;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Speed"))
	float m_Speed;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Offset"))
	float m_OffsetFromSpawn;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Circular Motion Circle Radius", EditCondition = "m_Speed == 0.0", EditConditionHides))
	float m_CircularMotionCircleRadius;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Circular Rotation Speed", EditCondition = "m_Speed == 0.0", EditConditionHides))
	float m_CircularRotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Starting Delay",
		ClampMin = "0.01", UIMin = "0.01"))
	float m_Delay;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Initial Z Rotation"))
	float m_BallInitialRotation;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Max Duration"))
	float m_MaxDuration;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Ball Size"))
	float m_BallSize;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Ball Color"))
	FColor m_Color;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Color Glow Strength"))
	float m_ColorGlowStrength;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Ball Can Bounce Of Walls"))
	bool m_BouncesOfWalls;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EDamageBallWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Custom Weapon Mesh Name", EditCondition = "m_WeaponMeshName == EDamageBallWeaponNames::MESH_CUSTOM", EditConditionHides))
	FName m_CustomWeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Animation Name"))
	EDamageBallAnimationNames m_AnimName;

	FName GetWeaponMeshName() const;
	void PlayDamageBall(AActor* a_AbilityUser);

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;

private:

	UAnimMontage* m_AttackMontage;
	FTimerHandle m_StartTimerHandle;
};
