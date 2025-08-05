// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include "Engine/StaticMeshActor.h"
#include "DamageCircleAbilityAction.generated.h"

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

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Knockback Strenght"))
	float m_KnockbackStrenght = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Circle Color"))
	FColor m_Color;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Circle Follows The User"))
	bool m_CircleFollowsUser;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Weapon Mesh Name"))
	EAAWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Damage Circle Action Settings", meta = (DisplayName = "Animation Name"))
	EAAAnimationNames m_AnimName;

	void DrawDebug(AActor* a_AbilityUser);
	void MoveCircle(AActor* a_AbilityUser);

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	virtual void EndAbilityAction(AActor* a_AbilityUser) override;

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
