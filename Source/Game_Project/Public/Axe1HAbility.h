// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Axe1HAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UAxe1HAbility : public UBaseAbility
{
	GENERATED_BODY()
public:
	UAxe1HAbility();

	virtual void UseAbility(AActor* a_AbilityUser) override;
	virtual void EquipAbility(AActor* a_AbilityUser) override;
};
