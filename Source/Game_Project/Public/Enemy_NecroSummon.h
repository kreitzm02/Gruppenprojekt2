// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Enemy_Necromancer.h"
#include "Enemy_NecroSummon.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AEnemy_NecroSummon : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnDeath() override;

	float GetRiseDuration() { return m_riseDuration; }

	void SetHeightOffset(float a_heightOffset) { m_heightOffset = a_heightOffset; }

	float GetHeightOffset() { return m_heightOffset; }

	void SetOwnerNecromancer(AEnemy_Necromancer* a_enemyNecromancer) { m_ownerNecro = a_enemyNecromancer; }

	UAnimSequence* GetRiseAnimation() { return m_riseAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_riseAnimation = nullptr;

	UPROPERTY()
	AEnemy_Necromancer* m_ownerNecro = nullptr;

	float m_riseDuration = 0.0f;

	float m_heightOffset = 0.0f;

	float m_normalHeight = 0.0f;
};
