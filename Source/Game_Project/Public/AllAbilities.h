// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllAbilities.generated.h"

UENUM()
enum class EAllAbilities : uint8
{
	SINGLEHEAL,
	MULTIHEAL,
	SINGLESTAMINA,
	MULTISTAMINA,
	STATICAOE,
	FOLLOWAOE,
	MULTIAOE,
	SINGLEPROJECTILE,
	MULTIPROJECTILE,
	BURSTPROJECTILE,
	ORBITPROJECTILE,
	SINGLETORNADO,
	MULTITORNARDO,
	CHAINLIGHTNING,
	MELEEAXE,
	MELEESWORD,
	RANGEDBOW,
	ENUMLENGTH UMETA(Hidden)
};
