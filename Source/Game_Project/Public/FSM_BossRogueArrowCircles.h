// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Rogue.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossRogueArrowCircles.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossRogueArrowCircles : public UFSM_BaseEnemyState
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltaTime) override;

	virtual void OnExit() override;


private:
	UPROPERTY()
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY()
	ABossEnemy_Rogue* m_thisEnemy;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shootAtAnimStartOffset = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_slowerMultiplier = 0.5f;

	UPROPERTY()
	FVector m_spawnPoint = FVector::OneVector;

	UPROPERTY()
	FVector m_shootDirection = FVector::OneVector;

	bool m_shotFired = false;

	bool m_movedToSpawn = false;

	float m_multiplier;

	float m_animDuration;

	float m_passedTime;
};
