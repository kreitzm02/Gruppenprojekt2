// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestGM.generated.h"

/**
 * 
 */
UCLASS()
class ATestGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATestGM();

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	TSubclassOf<APawn> m_PlayerPawnBPClass;
};
