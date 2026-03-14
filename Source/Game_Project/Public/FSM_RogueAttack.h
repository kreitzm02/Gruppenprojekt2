// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Rogue.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_RogueAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_RogueAttack : public UFSM_BaseEnemyState
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
	UAnimSequence* m_reloadAnimation = nullptr;

	UPROPERTY()
	AEnemy_Rogue* m_thisEnemy;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shootAtAnimStartOffset = 0.0f;

	UPROPERTY()
	AActor* m_target = nullptr;

	bool m_shotFired = false;

	bool m_weaponLoaded = true;

	float m_animDuration = 0.0f;

	float m_passedTime;
};
