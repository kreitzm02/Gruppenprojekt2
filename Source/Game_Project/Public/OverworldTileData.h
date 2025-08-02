// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OverworldTileData.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API UOverworldTileData : public UDataAsset
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "Data")
	UStaticMesh* m_tileMesh;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<bool> m_edgeHasRoad;

public:
	UStaticMesh* GetTileMeshPtr() { return m_tileMesh; }

	TArray<bool> GetEdgeData() { return m_edgeHasRoad; }
};
