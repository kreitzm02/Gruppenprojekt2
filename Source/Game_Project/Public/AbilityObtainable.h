// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllAbilities.h"
#include "UObject/Interface.h"
#include "AbilityObtainable.generated.h"

/**
 * 
 */
UINTERFACE()
class GAME_PROJECT_API UAbilityObtainable : public UInterface
{
	GENERATED_BODY()
	
};

class IAbilityObtainable
{
	GENERATED_BODY()
public:
	virtual int GetAbilityLevel(EAllAbilities a_Ability) = 0;
	virtual int GetMaxAbilitySlots() = 0;
	virtual int GetNextAbilitySlot() = 0;
};
