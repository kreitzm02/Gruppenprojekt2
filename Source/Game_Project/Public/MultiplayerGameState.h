// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiplayerGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeedReady, int32, a_seed);

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
	int32 m_worldSeed;

	UPROPERTY()
	FOnSeedReady OnSeedReady;

protected:
	UFUNCTION()
	void OnRep_WorldSeed();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
