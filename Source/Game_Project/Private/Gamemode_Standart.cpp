// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode_Standart.h"
#include "Engine/World.h"

AGamemode_Standart::AGamemode_Standart()
{
	bUseSeamlessTravel = true;
}

void AGamemode_Standart::LoadNewMap(FString a_mapName)
{
	if (HasAuthority())
	{
		DoServerTravel();
	}
	else
	{
		RequestServerTravel();
	}
}




void AGamemode_Standart::RequestServerTravel_Implementation()
{
	DoServerTravel();
}

void AGamemode_Standart::DoServerTravel()
{
	if (!HasAuthority())
	{
		return;
	}
	if (m_travelInProgress)
	{
		return;
	}
	m_travelInProgress = true;
	if (UWorld* world = GetWorld())
	{
		const ENetMode NetMode = GetNetMode();
		UE_LOG(LogTemp, Warning, TEXT("TRAVEL: NetMode=%d (0=Standalone,1=Dedicated,2=Listen,3=Client) HasAuthority=%d"),
			(int32)NetMode, HasAuthority() ? 1 : 0);
		world->ServerTravel("/Game/temp?listen");
	}
}