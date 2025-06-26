// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Enemy_Golem.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AEnemy_Golem : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	

	virtual void Tick(float DeltaTime) override;

	void CreateShockwave();

	bool GetSmashReady() { return m_smashReady; }

	void SetSmashDuration(float a_smashDuration) { m_smashDuration = a_smashDuration; }

	float GetSmashDuration() { return m_smashDuration; }

	void SetSmashReady(bool a_chargeReady) { m_smashReady = a_chargeReady; }

	UAnimSequence* GetSmashAnimation() { return m_smashAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_smashAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_abilityCooldown = 20.0f;

	float m_passedCooldownTime = 0.0f;

	float m_smashDuration = 0.0f;

	bool m_smashReady = true;
};
