// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiplayerGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverworldSeedReady, int32, a_seed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonSeedReady, int32, a_seed);

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AMultiplayerGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMultiplayerGameState();

	UPROPERTY(ReplicatedUsing = OnRep_WorldSeed, BlueprintReadOnly)
	int32 m_overworldSeed;

	UPROPERTY()
	FOnOverworldSeedReady OnOverworldSeedReady;

	UPROPERTY(ReplicatedUsing = OnRep_DungeonSeed, BlueprintReadOnly)
	int32 m_dungeonSeed;

	UPROPERTY()
	FOnOverworldSeedReady OnDungeonSeedReady;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_WorldSeed();

	UFUNCTION()
	void OnRep_DungeonSeed();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
