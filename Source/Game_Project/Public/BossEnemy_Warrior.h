// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float GetChargeDuration() { return m_chargeDuration; }

	float GetChargeSpeed() { return m_baseChargeSpeed * m_currentDoStuffMultiplier; }

	bool GetChargeReady() { return m_chargeReady; }

	void SetChargeReady(bool a_chargeReady) { m_chargeReady = a_chargeReady; }

	UAnimSequence* GetChargeAnimation() { return m_chargeAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_chargeAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_spinAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_chargeDuration = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_chargeCooldown = 20.0f;

	float m_passedCooldownTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_baseChargeSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxMultiplierAtHPPercent = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_doStuffMaxMultiplier = 4.0f;

	float m_currentDoStuffMultiplier = 1.0f;

	bool m_chargeReady = true;
};
