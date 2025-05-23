// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "OverworldTileData.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
		UStaticMesh* tileMesh = nullptr;
		FRotator rotation = FRotator::ZeroRotator;
		TArray<bool> edgeHasRoad = {};

		TileRoadWithRotationData(UStaticMesh* a_staticMesh, FRotator a_rotation, TArray<bool> a_edgeHasRoad)
		{
			tileMesh = a_staticMesh;
			rotation = a_rotation;
			edgeHasRoad = a_edgeHasRoad;
		}
	};

	struct PositionAndEdgeData
	{
		FVector position = FVector::ZeroVector;
		TArray<bool> edgeHasRoad = {};

		PositionAndEdgeData();
		PositionAndEdgeData(FVector a_position, TArray<bool> a_edgeHasRoad)
		{
			position = a_position;
			edgeHasRoad = a_edgeHasRoad;
		}
	};


	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	int tileSize = 100;

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	int32 seed = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	int worldSize = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
	int startRoadCountMin = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
	int startRoadCountMax = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int roadSpreadPossability = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int roadStraightness = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int natureDensity = 0;

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	TArray<UOverworldTileData*> tileData = {};

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	TArray<UStaticMesh*> worldBorder = {};

	UPROPERTY(EditAnywhere, Category = "Overworld settings")
	TArray<UStaticMesh*> natureDecorations = {};

	//UPROPERTY(EditAnywhere, Category = "Overworld settings")
	//TArray<ADungeonEntrance*> dungeonEntrance = {};

	//UPROPERTY(EditAnywhere, Category = "Overworld settings")
	//TArray<AEnemySpawn*> dungeonEntrance = {};

	FRandomStream randomNumber;

	TArray<PositionAndEdgeData> lastGeneratedTiles = {};
	TArray<PositionAndEdgeData> currentGeneratedTiles = {};


	TArray<FVector>* endTiles = new TArray<FVector>;
	TArray<FVector>* emptyTiles = new TArray<FVector>;

	//map for each tile with each rotation and associated road data ordered after road count of roads on each edge
	TMap<int, TArray<TArray<TileRoadWithRotationData>>>* possibleTilesMap = new TMap<int, TArray<TArray<TileRoadWithRotationData>>>();


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
