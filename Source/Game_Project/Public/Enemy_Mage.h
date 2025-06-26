// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Enemy_Mage.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AEnemy_Mage : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void FireProjectile(FVector a_direction);

	void FireFireball(ACharacter* a_playerToFollow);

	bool GetAbilityReady() { return m_abilityReady; }

	void SetAbilityReady(bool a_chargeReady) { m_abilityReady = a_chargeReady; }

	void SetFireballCastTime(float a_castTime) { m_fireballCastTime = a_castTime; }

	float GetFireballCastTime() { return m_fireballCastTime; }

	UAnimSequence* GetCastingAnimation() { return m_castingAnimation; }

	UAnimSequence* GetCastShootAnimation(){ return m_castShootAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_castingAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_castShootAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_abilityCooldown = 20.0f;

	float m_passedCooldownTime = 0.0f;

	bool m_abilityReady = true;

	float m_fireballCastTime = 0.0f;
};
