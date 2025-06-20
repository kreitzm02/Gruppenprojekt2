// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "OverworldTileData.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "OverworldGeneration.generated.h"


UCLASS()
class GAME_PROJECT_API AOverworldGeneration : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverworldGeneration();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	enum TileType
	{
		RoadNoEdges,
		RoadEnd,
		RoadSmallCurve,
		RoadBigCurve,
		RoadStraight,
		RoadE,
		RoadYLeft,
		RoadYRight,
		RoadYWide,
		RoadX,
		RoadK,
		RoadYPlusOne,
		RoadFiveEdges,
		RoadAllEdges
	};

	struct TileRoadWithRotationData
	{
	private:
		TileRoadWithRotationData();
	public:
		UStaticMesh* m_tileMesh = nullptr;
		FRotator m_rotation = FRotator::ZeroRotator;
		TArray<bool> m_edgeHasRoad = {};

		TileRoadWithRotationData(UStaticMesh* a_staticMesh, FRotator a_rotation, TArray<bool> a_edgeHasRoad)
		{
			m_tileMesh = a_staticMesh;
			m_rotation = a_rotation;
			m_edgeHasRoad = a_edgeHasRoad;
		}
	};

	struct PositionAndEdgeData
	{
		FVector m_position = FVector::ZeroVector;
		TArray<bool> m_edgeHasRoad = {};

		PositionAndEdgeData();
		PositionAndEdgeData(FVector a_position, TArray<bool> a_edgeHasRoad)
		{
			m_position = a_position;
			m_edgeHasRoad = a_edgeHasRoad;
		}
	};


	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	int m_tileSize = 100;

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	int32 m_seed = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	int m_worldSize = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
	int m_startRoadCountMin = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
	int m_startRoadCountMax = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int m_roadSpreadPossability = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int m_roadStraightness = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int m_natureDensity = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	TArray<UOverworldTileData*> m_tileData = {};

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	TArray<UStaticMesh*> m_worldBorder = {};

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	TArray<UStaticMesh*> m_natureDecorations = {};

	UPROPERTY(EditAnywhere, Category = "AIMap")
	ANavMeshBoundsVolume* m_navMesh;

	//UPROPERTY(EditAnywhere, Category = "Overworld settings")
	//TArray<ADungeonEntrance*> dungeonEntrance = {};

	//UPROPERTY(EditAnywhere, Category = "Overworld settings")
	//TArray<AEnemySpawn*> dungeonEntrance = {};

	FRandomStream m_randomNumber;

	TArray<PositionAndEdgeData> m_lastGeneratedTiles = {};
	TArray<PositionAndEdgeData> m_currentGeneratedTiles = {};


	TArray<FVector>* m_endTiles = new TArray<FVector>;
	TArray<FVector>* m_emptyTiles = new TArray<FVector>;

	//map for each tile with each m_rotation and associated road data ordered after road count of roads on each edge
	TMap<int, TArray<TArray<TileRoadWithRotationData>>>* m_possibleTilesMap = new TMap<int, TArray<TArray<TileRoadWithRotationData>>>();


	void InitializeTMap();

	TArray<TileRoadWithRotationData> GetRotatedTileAndEdges(UOverworldTileData* a_tileData, int& a_roadCount);

	FVector TilePosition(int a_width, int a_height);

	TArray<FVector> NextPositionInWorldSpace(int a_circleIteration);

	TOptional<bool> GetNeighboringTilesEdgeData(FVector a_currentPos, FVector a_offset, int a_edgeToCheck);

	TArray<TOptional<bool>> GetNeededEdgeData(FVector a_currentPos, TArray<FVector> a_offsets);

	void GetPossibleRoadCount(TArray<TOptional<bool>> a_neededEdgeData, int& a_minRoadCount, int& a_maxRoadCount);

	//bool IsTilePossible(TArray<TileRoadWithRotationData>);

	bool IsTileTileType(TileRoadWithRotationData a_tile, TileType a_tiletype);
};
