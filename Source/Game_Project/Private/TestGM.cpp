// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGM.h"
#include "Player/PlayerCharacter.h"
#include "DefaultPlayerController.h"
#include "MultiplayerGameState.h"

ATestGM::ATestGM()
{
	DefaultPawnClass = m_PlayerPawnBPClass;
	PlayerControllerClass = ADefaultPlayerController::StaticClass();
}

void ATestGM::BeginPlay()
{
	Super::BeginPlay();

	

	AMultiplayerGameState* gs = GetGameState<AMultiplayerGameState>();

	if (gs)
	{
		int32 seed = FMath::RandRange(1,RAND_MAX);

		gs->m_worldSeed = seed;

		gs->ForceNetUpdate();

		UE_LOG(LogTemp, Warning, TEXT("Host set Seed: %d"), seed);

		gs->OnSeedReady.Broadcast(seed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Game State found"));
	}
}
