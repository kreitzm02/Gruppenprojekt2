// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIController.h"
#include "BaseStateTransition.generated.h"

class UBaseEnemyState;
/**
 * 
 */
UCLASS(EditInlineNew)
class GAME_PROJECT_API UBaseStateTransition : public UObject
{
	GENERATED_BODY()

public:

    virtual void Initialize();
    virtual bool IsConditionMet();
};
