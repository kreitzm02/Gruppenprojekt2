// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonExitPortal.h"
#include "EnemyCharacter.h"
#include "Components/ArrowComponent.h"
#include "BossEnemy_Mage.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ABossEnemy_Mage : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	ABossEnemy_Mage();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float a_damageAmount, FDamageEvent const& a_damageEvent, AController* a_eventInstigator, AActor* a_damageCauser) override;

	void PlayCastFireballSound(bool a_shouldLoop = false, float a_startPoint = 0.0f, float a_soundDuration = 0.0f);

	void PlayCastBurnGroundSound(bool a_shouldLoop = false, float a_startPoint = 0.0f, float a_soundDuration = 0.0f);

	void FireProjectile(AActor* a_target);

	void FireFireball(AActor* a_target);

	void FireBurnGround(ACharacter* a_target);

	float GetMultiplier() { return m_currentDoStuffMultiplier; }

	int GetMaxBurnGroundsPerAbility() { return m_burnGroundsPerAbility; }

	void AddUsedBurnGround() { m_currentBurnGround += 1; }

	void ResetUsedBurnGrounds() { m_currentBurnGround = 0; }

	int GetCurrentBurnGround() { return m_currentBurnGround; }

	bool GetBurnGroundReady() { return m_burnGroundReady; }

	bool GetFireballReady() { return m_fireballReady; }

	void SetBurnGroundReady(bool a_burnGroundReady) { m_burnGroundReady = a_burnGroundReady; }

	void SetFireballReady(bool a_fireballReady) { m_fireballReady = a_fireballReady; }

	void SetFireballCastTime(float a_castTime) { m_fireballCastTime = a_castTime; }

	float GetFireballCastTime() { return m_fireballCastTime; }

	UAnimSequence* GetBurnGroundAnimation() { return m_burnGroundAnimation; }

	UAnimSequence* GetFireballCastAnimation() { return m_fireballCastAnimation; }

	UAnimSequence* GetFireballCastingAnimation() { return m_fireballCastingAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Scale")
	FVector m_weaponScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_burnGroundAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_fireballCastAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_fireballCastingAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_projectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_projectileLifetime = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_projectileBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_fireballSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_fireballLifetime = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_fireballBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int m_burnGroundsPerAbility = 5;

	int m_currentBurnGround = 0;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_burnGroundLifetime = 5;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_burnGroundFlightTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_burnGroundBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_abilityCooldown = 20.0f;

	float m_passedCooldownTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxMultiplierAtHPPercent = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_doStuffMaxMultiplier = 4.0f;

	float m_currentDoStuffMultiplier = 1.0f;

	bool m_burnGroundReady = false;

	bool m_fireballReady = false;

	UPROPERTY(EditAnywhere)
	UArrowComponent* m_projectileSpawnPoint = nullptr;

	float m_fireballCastTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	TSubclassOf<ADungeonExitPortal> m_dungeonExitBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	USoundWave* m_castFireballSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	USoundWave* m_castBurnGroundSound = nullptr;
};
