// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Mage.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_MageFireball.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_MageFireball : public UFSM_BaseEnemyState
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltaTime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_castAnimation = nullptr;

	UPROPERTY()
	UAnimSequence* m_castShootAnimation = nullptr;

	UPROPERTY()
	AEnemy_Mage* m_thisEnemy;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shootAtAnimStartOffset = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int m_castingLoopsUntilFireball = 1;

	UPROPERTY()
	AActor* m_player = nullptr;

	bool m_fireballFired = false;

	bool m_castShootStarted = false;

	float m_castDuration = 0.0f;

	float m_passedTime = 0.0f;
};
