// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Enemy_Necromancer.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AEnemy_Necromancer : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void FireProjectile(FVector a_direction);

	void Summon();

	bool GetSummonReady() { return m_summonReady; }

	void SetSummonReady(bool a_chargeReady) { m_summonReady = a_chargeReady; }

	int GetMaxSummons(){ return m_maxSummons; }

	int GetCurrentSummons(){ return m_currentSummons; }

	UAnimSequence* GetSummonAnimation() { return m_summonAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_summonAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_summonCooldown = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int m_maxSummons = 4;

	int m_currentSummons = 0;

	float m_passedCooldownTime = 0.0f;

	bool m_summonReady = true;
};
