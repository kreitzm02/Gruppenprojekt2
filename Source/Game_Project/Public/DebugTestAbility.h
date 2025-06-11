// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "DebugTestAbility.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GAME_PROJECT_API UDebugTestAbility : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	UDebugTestAbility();

	virtual void UseAbility(AActor* a_AbilityUser) override;
	virtual void EquipAbility(AActor* a_AbilityUser) override;
};
