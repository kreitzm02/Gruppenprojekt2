// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Enemy_Warrior.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AEnemy_Warrior : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	float GetChargeDuration() { return chargeDuration; }

	float GetChargeSpeed() { return chargeSpeed; }

	bool GetChargeReady() { return chargeReady; }

	void SetChargeReady(bool a_chargeReady) { chargeReady = a_chargeReady; }

	UAnimSequence* GetChargeAnimation() { return chargeAnimation; }

	UAnimSequence* GetAttackAnimation() { return attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* chargeAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float chargeDuration = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float chargeCooldown = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float chargeSpeed = 400.0f;

	float passedCooldownTime = 0.0f;

	bool chargeReady = true;
};
