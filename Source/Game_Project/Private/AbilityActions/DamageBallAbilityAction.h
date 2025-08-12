// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <NiagaraSystem.h>
#include "NiagaraComponent.h"
#include "AbilityActions/BaseAbilityAction.h"
#include "DamageBallAbilityAction.generated.h"


struct FDamageBallInstance
{
	FVector m_Direction;
	FVector m_CurrPosition;
	int32 m_HitCount = 0;
	TSet<AActor*> m_AlreadyHitActors;
	float m_CircleAngle;
	UNiagaraComponent* m_VFXComp = nullptr;
	FTimerHandle m_MoveHandle;
	FTimerHandle m_EndHandle;
};

UCLASS(Blueprintable, EditInlineNew)
class UDamageBallAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Niagara VFX", meta = (DisplayName = "Projectile"))
	UNiagaraSystem* m_VFX;

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

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Starting Delay", ClampMin = "0.01", UIMin = "0.01"))
	float m_Delay;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Initial Z Rotation"))
	float m_BallInitialRotation;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Max Duration"))
	float m_MaxDuration;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Ball Size"))
	float m_BallSize;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Knockback Strenght"))
	float m_KnockbackStrenght = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Color Glow Strength"))
	float m_ColorGlowStrength;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Ball Can Bounce Of Walls"))
	bool m_BouncesOfWalls;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EAAWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Damage Ball Action Settings", meta = (DisplayName = "Animation Name"))
	EAAAnimationNames m_AnimName;

	void PlayDamageBall(AActor* a_AbilityUser);

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	virtual void EndAbilityAction(AActor* a_AbilityUser) override;

private:

	UAnimMontage* m_AttackMontage;
	FTimerHandle m_StartTimerHandle;
	bool forceDestroyActive = false;
	void MoveDamageBallTick(TSharedPtr<FDamageBallInstance> a_BallInstance, AActor* a_AbilityUser);
	void EndDamageBall(TSharedPtr<FDamageBallInstance> a_BallInstance);
	void EndAllDamageBalls();
	TArray<TSharedPtr<FDamageBallInstance>> m_ActiveBalls;
};
