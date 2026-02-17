// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Gamemode_Standart.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AGamemode_Standart : public AGameMode
{
	GENERATED_BODY()

public:
	AGamemode_Standart();

	void LoadNewMap(FString a_mapName);

private:
	bool m_travelInProgress = false;

	UFUNCTION(Server, Reliable)
	void RequestServerTravel();

	void DoServerTravel();

};
