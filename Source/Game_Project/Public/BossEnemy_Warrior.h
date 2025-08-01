// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonExitPortal.h"
#include "EnemyCharacter.h"
#include "BossEnemy_Warrior.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ABossEnemy_Warrior : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float a_damageAmount, FDamageEvent const& a_damageEvent, AController* a_eventInstigator, AActor* a_damageCauser) override;

	float GetMultiplier() { return m_currentDoStuffMultiplier; }

	float GetChargeOnPointDuration() { return m_chargeOnPointDuration; }

	int GetMaxChargesPerAbility() { return m_chargesPerChargeAbility; }

	void AddUsedCharge() { m_currentCharges += 1; }

	void ResetUsedCharges() { m_currentCharges = 0; }

	int GetCurrentCharges() { return m_currentCharges; }

	float GetSpinDuration() { return m_spinDuration; }

	float GetChargeSpeed() { return m_baseChargeSpeed; }

	float GetSpinSpeed() { return m_baseSpinSpeed; }

	bool GetChargeReady() { return m_chargeReady; }

	bool GetSpinReady() { return m_spinReady; }

	void SetChargeReady(bool a_chargeReady) { m_chargeReady = a_chargeReady; }

	void SetSpinReady(bool a_spinReady) { m_spinReady = a_spinReady; }

	UAnimSequence* GetChargeAnimation() { return m_chargeAnimation; }

	UAnimSequence* GetSpinAnimation() { return m_spinAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Scale")
	FVector m_weaponScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_chargeAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_spinAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_chargeOnPointDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int m_chargesPerChargeAbility = 5;

	int m_currentCharges = 0;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_spinDuration = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_abilityCooldown = 20.0f;

	float m_passedCooldownTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_baseChargeSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_baseSpinSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxMultiplierAtHPPercent = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_doStuffMaxMultiplier = 4.0f;

	float m_currentDoStuffMultiplier = 1.0f;

	bool m_chargeReady = false;

	bool m_spinReady = false;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	TSubclassOf<ADungeonExitPortal> m_dungeonExitBP = nullptr;
};
