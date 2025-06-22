// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSM_BaseStateTransition.h"
#include "FSM_BaseEnemyState.h"
#include "Engine/DataAsset.h"
#include "FSMData.generated.h"

/**
 * 
 */
USTRUCT()
struct FTargetStateWithCondition
{//contains  one target state with one condition to reach that state
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced)
	UFSM_BaseStateTransition* m_targetCondition;

	UPROPERTY(EditAnywhere, Instanced)
	UFSM_BaseEnemyState* m_targetState;
};

USTRUCT()
struct FStateData
{//contains one origin state and a list with target states and their related conditions
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced)
	UFSM_BaseEnemyState* m_originState;

	UPROPERTY(EditAnywhere)
	TArray<FTargetStateWithCondition> m_targetStateWithCond;
};



UCLASS()
class GAME_PROJECT_API UFSMData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FStateData> m_stateData;

public:
	TArray<FStateData> GetStateData()
	{
		return m_stateData;
	}
};
