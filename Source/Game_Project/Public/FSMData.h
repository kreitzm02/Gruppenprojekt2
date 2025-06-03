// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStateTransition.h"
#include "BaseEnemyState.h"
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
	UBaseStateTransition* targetCondition;

	UPROPERTY(EditAnywhere, Instanced)
	UBaseEnemyState* targetState;
};

USTRUCT()
struct FStateData
{//contains one origin state and a list with target states and their related conditions
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced)
	UBaseEnemyState* originState;

	UPROPERTY(EditAnywhere)
	TArray<FTargetStateWithCondition> targetStateWithCond;
};



UCLASS()
class GAME_PROJECT_API UFSMData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FStateData> stateData;

public:
	TArray<FStateData> GetStateData()
	{
		return stateData;
	}
};
