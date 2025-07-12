// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy_Warrior.h"
#include "FSM_BaseEnemyState.h"
#include "FSM_BossWarriorSpin.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UFSM_BossWarriorSpin : public UFSM_BaseEnemyState
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;

	virtual void OnEnter() override;

	virtual void OnUpdate(float a_deltatime) override;

	virtual void OnExit() override;

private:
	UPROPERTY()
	UAnimSequence* m_chargeAnimation;

	UPROPERTY()
	ACharacter* m_player = nullptr;

	UPROPERTY()
	ABossEnemy_Warrior* m_owner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool m_enableDebug = true;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_changeDirBeforeWallMultiplier = 1.0f;

	float m_spinSpeed = 0.0f;

	float m_detectionRange = 0.0f;

	FVector m_spinDirectionNormal = FVector::ZeroVector;
};
