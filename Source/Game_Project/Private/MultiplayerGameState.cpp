// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameState.h"

#include "Net/UnrealNetwork.h"

AMultiplayerGameState::AMultiplayerGameState()
{
	m_worldSeed = 0;
}

void AMultiplayerGameState::OnRep_WorldSeed()
{
	UE_LOG(LogTemp, Warning, TEXT("Client received Seed: %d"), m_worldSeed);

	OnSeedReady.Broadcast(m_worldSeed);
}

void AMultiplayerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerGameState, m_worldSeed);
}
