// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameState.h"

#include "Game_GameInstance.h"
#include "Net/UnrealNetwork.h"

AMultiplayerGameState::AMultiplayerGameState()
{
	m_overworldSeed = 0;
}

void AMultiplayerGameState::BeginPlay()
{
	UGame_GameInstance* gm = Cast<UGame_GameInstance>(GetGameInstance());

	if (gm->m_overworldSeedSet)
	{
		m_overworldSeed = gm->m_overworldSeed;
		OnRep_WorldSeed();
		gm->m_overworldSeedSet = false;
	}
}


void AMultiplayerGameState::OnRep_WorldSeed()
{
	UE_LOG(LogTemp, Warning, TEXT("Client received Seed: %d"), m_overworldSeed);

	OnOverworldSeedReady.Broadcast(m_overworldSeed);
}

void AMultiplayerGameState::OnRep_DungeonSeed()
{
	UE_LOG(LogTemp, Warning, TEXT("Client received Seed: %d"), m_dungeonSeed);

	OnOverworldSeedReady.Broadcast(m_dungeonSeed);
}


void AMultiplayerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerGameState, m_overworldSeed);

	DOREPLIFETIME(AMultiplayerGameState, m_dungeonSeed);
}
