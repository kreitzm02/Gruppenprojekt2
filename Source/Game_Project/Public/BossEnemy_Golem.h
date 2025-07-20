// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "BossEnemy_Golem.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ABossEnemy_Golem : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	ABossEnemy_Golem();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float GetMultiplier() { return m_currentDoStuffMultiplier; }

	int GetMaxJumpsPerAbility() { return m_jumpsPerChargeAbility; }

	void AddUsedJump() { m_currentJumps += 1; }

	void ResetUsedJump() { m_currentJumps = 0; }

	int GetCurrentJumps() { return m_currentJumps; }

	int GetMaxSmashesPerAbility() { return m_smashesPerChargeAbility; }

	void AddUsedSmash() { m_currentSmashes += 1; }
	
	void ResetUsedSmash() { m_currentSmashes = 0; }

	int GetCurrentSmashes() { return m_currentSmashes; }

	void CreateSmashShockwaveWithBoulder(AActor* a_boulderTarget);

	void CreateJumpShockwave();

	bool GetSmashReady() { return m_smashReady; }

	bool GetJumpReady() { return m_jumpReady; }

	void SetSmashReady(bool a_chargeReady) { m_smashReady = a_chargeReady; }

	void SetJumpReady(bool a_jumpReady) { m_jumpReady = a_jumpReady; }

	UAnimSequence* GetSmashAnimation() { return m_smashAnimation; }

	UAnimSequence* GetJumpAnimation() { return m_jumpAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Scale")
	FVector m_weaponScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_smashAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_jumpAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_abilityCooldown = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_shockwaveBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_boulderBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_boulderHeightOffset = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_shockwaveExpansionSpeed = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_shockwaveMaxRadius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_shockwaveStartRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int m_jumpsPerChargeAbility = 4;

	int m_currentJumps = 0;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int m_smashesPerChargeAbility = 3;

	int m_currentSmashes = 0;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxMultiplierAtHPPercent = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_doStuffMaxMultiplier = 4.0f;

	float m_currentDoStuffMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UArrowComponent* m_shockwaveWeaponSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UArrowComponent* m_shockwaveJumpSpawnPoint = nullptr;

	float m_passedCooldownTime = 0.0f;

	float m_smashDuration = 0.0f;

	bool m_smashReady = false;

	bool m_jumpReady = false;
};
