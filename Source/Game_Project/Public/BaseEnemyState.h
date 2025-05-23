// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseEnemyState.generated.h"

class UBaseStateTransition;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class GAME_PROJECT_API UBaseEnemyState : public UObject
{
	GENERATED_BODY()

public:
    void OnEnter();
    
    void OnExit();
    
    void TickState(float DeltaTime);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<class UBaseStateTransition*> transitions;
};
