// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Golem.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossGolemAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossGolemAttack : public UFSM_BaseEnemyState
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
	ABossEnemy_Golem* m_thisEnemy;

	UPROPERTY()
	AActor* m_player = nullptr;
};
