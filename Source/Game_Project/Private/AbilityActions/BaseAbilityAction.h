// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAbilityAction.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class UBaseAbilityAction : public UObject
{
	GENERATED_BODY()
	
public:

	virtual void PrepareAbilityAction(AActor* a_AbilityUser);
	virtual void PlayAbilityAction(AActor* a_AbilityUser);
};
