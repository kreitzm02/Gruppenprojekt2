// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIController.h"
#include "FSM_BaseStateTransition.generated.h"

class UFSM_BaseEnemyState;
/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable)
class GAME_PROJECT_API UFSM_BaseStateTransition : public UObject
{
	GENERATED_BODY()

public:

    virtual void Initialize();
    virtual void ResetCondition();
    virtual bool IsConditionMet(float a_deltaTime);
};
