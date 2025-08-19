// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityActions/BaseAbilityAction.h"
#include <NiagaraSystem.h>
#include "NiagaraComponent.h"
#include "TornadoAbilityAction.generated.h"

struct FTornadoInstance
{
	FVector m_Direction;
	FVector m_CurrPosition;
	TSet<AActor*> m_AlreadyHitActors;
	UNiagaraComponent* m_VFXComp = nullptr;
	UAudioComponent* m_SoundComp = nullptr;
	FTimerHandle m_MoveHandle;
	FTimerHandle m_EndHandle;
	FTimerHandle m_ClearHitListHandle;
};

UCLASS()
class UTornadoAbilityAction : public UBaseAbilityAction
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Niagara VFX", meta = (DisplayName = "Tornado vfx"))
	UNiagaraSystem* m_TornadoVFX;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Sound Effect"))
	USoundWave* m_SoundEffect;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Spawn Radius"))
	float m_SpawnRadius = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Damage"))
	float m_Damage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Attraction Radius"))
	float m_AttractionRadius = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Attraction Strength"))
	float m_AttractionStrength = 1000.0f;

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
	EAAWeaponNames m_WeaponMeshName;

	UPROPERTY(EditAnywhere, Category = "Tornado Settings", meta = (DisplayName = "Animation Name"))
	EAAAnimationNames m_AnimName;

	virtual void PrepareAbilityAction(AActor* a_AbilityUser) override;
	virtual void PlayAbilityAction(AActor* a_AbilityUser) override;
	virtual void EndAbilityAction(AActor* a_AbilityUser) override;

private:

	void PlayTornado(AActor* a_AbilityUser);
	void MoveTornadoTick(TSharedPtr<FTornadoInstance> a_Instance);
	void UpdateHitActors(TSharedPtr<FTornadoInstance> a_Instance);
	void EndTornado(TSharedPtr<FTornadoInstance> a_Instance);
	void EndAllTornados();
	UAnimMontage* m_AttackMontage;
	FTimerHandle m_StartTimerHandle;
	TArray<TSharedPtr<FTornadoInstance>> m_ActiveTornados;
 };
