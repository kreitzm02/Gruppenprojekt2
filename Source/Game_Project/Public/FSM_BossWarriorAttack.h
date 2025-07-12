// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Warrior.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossWarriorAttack.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossWarriorAttack : public UFSM_BaseEnemyState
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
	ABossEnemy_Warrior* m_thisEnemy = nullptr;
};
