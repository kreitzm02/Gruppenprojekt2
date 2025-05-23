// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldGeneration.h"

#include "IContentBrowserSingleton.h"
#include "Engine/StaticMeshActor.h"
#include <Eigen/Core>
#include <Eigen/Geometry>

// Sets default values
AOverworldGeneration::AOverworldGeneration()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AOverworldGeneration::BeginPlay()
{
	Super::BeginPlay();

    if (seed == 0)
    {
        seed = FMath::Rand();
        UE_LOG(LogTemp, Log, TEXT("Random Seed: %d"), seed);
    }
    randomNumber.Initialize(seed);

    InitializeTMap();

    AStaticMeshActor* tileActor = nullptr;

    TArray<FVector> offset = {};
    
    for (int i = 0; i <= worldSize + 10; i++)
    {
        if (i == 0)
        {
            //generate starting tile
            int roadCountIndex = randomNumber.RandRange(startRoadCountMin, startRoadCountMax);
            int tileIndex = randomNumber.RandRange(0, (*possibleTilesMap)[roadCountIndex].Num() - 1);
            int tileRotationIndex = randomNumber.RandRange(0, (*possibleTilesMap)[roadCountIndex][tileIndex].Num() - 1);
            tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector, (*possibleTilesMap)[roadCountIndex][tileIndex][tileRotationIndex].rotation);
            if (tileActor)
            {
                tileActor->GetStaticMeshComponent()->SetStaticMesh((*possibleTilesMap)[roadCountIndex][tileIndex][tileRotationIndex].tileMesh);
                tileActor->SetMobility(EComponentMobility::Static);
            }
            PositionAndEdgeData tempData = PositionAndEdgeData(FVector(0,0,0), (*possibleTilesMap)[roadCountIndex][tileIndex][tileRotationIndex].edgeHasRoad);
            lastGeneratedTiles.Add(tempData);
            currentGeneratedTiles.Add(tempData);
            
        }
        else if (i <= worldSize)
        {
            TArray<FVector> worldPositions = NextPositionInWorldSpace(i);
        
            for (FVector a_pos : worldPositions)
            {
                //sets different offset vectors based on y coordinate
                //offset need to be around a tile starting bron top left in clockwise order
                if (a_pos.Y == 0)
                {
	                offset = 
                    {
                        FVector(0,1,0),
                        FVector(1,0,0),
                        FVector(1,-1,0),
                        FVector(0,-1,0),
                        FVector(-1,0,0),
                        FVector(-1,1,0)
	                };
                }
                else if (FMath::Fmod(a_pos.Y, 2.0f) == 0)
                {
                    if (a_pos.Y < 0)
                    {
                        offset = 
                        {
                            FVector(1, 1, 0),
                            FVector(1, 0, 0),
                            FVector(1, -1, 0),
                            FVector(0, -1, 0),
                            FVector(-1, 0, 0),
                            FVector(0, 1, 0),
                        };
                    }
                    if (a_pos.Y > 0)
                    {
                        offset = 
                        {
                            FVector(0, 1, 0),
                            FVector(1, 0, 0),
                            FVector(0, -1, 0),
                            FVector(-1, -1, 0),
                            FVector(-1, 0, 0),
                            FVector(-1, 1, 0),
                        };
                    }
                }
                else
                {
                    if (a_pos.Y < 0)
                    {
                        offset = 
                        {
                            FVector(0, 1, 0),
                            FVector(1, 0, 0),
                            FVector(0, -1, 0),
                            FVector(-1, -1, 0),
                            FVector(-1, 0, 0),
                            FVector(-1, 1, 0),
                        };
                    }
                    if (a_pos.Y > 0)
                    {
                        offset = 
                        {
                            FVector(1, 1, 0),
                            FVector(1, 0, 0),
                            FVector(1, -1, 0),
                            FVector(0, -1, 0),
                            FVector(-1, 0, 0),
                            FVector(0, 1, 0),
                        };
                    }
                }

                //returns the edge data that the next tile needs to have
				TArray<TOptional<bool>> neededEdgeData = GetNeededEdgeData(a_pos,offset);

                
                int minRoadCount = 0;
                int maxRoadCount = 0;
				GetPossibleRoadCount(neededEdgeData, minRoadCount, maxRoadCount);
                
                //no road tiles always have tiles without roads as neighbors
                if (minRoadCount == 0)
                {
                    maxRoadCount = 0;
                }

                //dont get tile with more roads than needed on edge of world
                if (i == worldSize - 1 && minRoadCount > 0)
                {
                    maxRoadCount = minRoadCount;
                }

                //possibility of road spreading
                if (minRoadCount <= 2 && maxRoadCount >= 2 && i < worldSize - 1)
                {
                    int roadSpreadRandom = randomNumber.RandRange(1, 100);
                    if (roadSpreadRandom >= roadSpreadPossability)
                    {
                        minRoadCount = 2;
                        maxRoadCount = 3;
                    }
                    else
                    {
                        minRoadCount = 3;
                        //maxRoadCount = 6;
                    }
                }

                TArray<TileRoadWithRotationData> possibleTiles;

                for (int j = minRoadCount; j <= maxRoadCount; j++)
                {
                    if (j == 1 && i <= worldSize - 2)//no single road tile until end of world
                    {
                        j++;
                    }
                    for(TArray<TileRoadWithRotationData> a_tileArray : (*possibleTilesMap)[j])
                    {
                        for(TileRoadWithRotationData a_tile : a_tileArray)//compare tiles bool array with neededEdgeData but ignoring the null data
                        {
                            for (int k = 0; k < a_tile.edgeHasRoad.Num(); k++)
                            {
                                if (!neededEdgeData[k].IsSet())
                                {
                                    
                                }
                                else if (neededEdgeData[k] != a_tile.edgeHasRoad[k])
                                {
                                    break;
                                }
                                if (k == a_tile.edgeHasRoad.Num() - 1)
                                {
                                    possibleTiles.Add(a_tile);
                                }
                            }
                        }
                    }
                }

                int selectedTile = 0;
                int roadStraightnessRandom = randomNumber.RandRange(1, 100);
                if (roadStraightnessRandom <= roadStraightness)
                {
                    selectedTile = 0;
                    for (TileRoadWithRotationData a_tile : possibleTiles)
                    {
                        
                        if (IsTileTileType(a_tile, RoadStraight))
                        {
                            break;
                        }
                        selectedTile++;
                    }

                    if (selectedTile >= possibleTiles.Num())
                    {
                        selectedTile = randomNumber.RandRange(0, possibleTiles.Num() - 1);
                    }
                }
                else
                {
                    selectedTile = 0;
                    TArray<int> bigCurveRoadTiles = {};
                    TArray<int> threeRoadTiles = {};
                    for (TileRoadWithRotationData a_tile : possibleTiles)
                    {
                        
                        if (IsTileTileType(a_tile, RoadBigCurve))
                        {
                            bigCurveRoadTiles.Add(selectedTile);
                        }
                    	else if (IsTileTileType(a_tile, RoadE) || IsTileTileType(a_tile, RoadYLeft) || IsTileTileType(a_tile, RoadYRight) || IsTileTileType(a_tile, RoadYWide))
                        {
                            threeRoadTiles.Add(selectedTile);
                        }
                        selectedTile++;
                    }

                    if (bigCurveRoadTiles.Num() > 0)
                    {
                        selectedTile = bigCurveRoadTiles[randomNumber.RandRange(0, bigCurveRoadTiles.Num() - 1)];
                    }
                    else if (threeRoadTiles.Num() > 0)
                    {
                        selectedTile = threeRoadTiles[randomNumber.RandRange(0, threeRoadTiles.Num() - 1)];
                    }
                    else
                    {
                        selectedTile = randomNumber.RandRange(0, possibleTiles.Num() - 1);
                    }
                }

                if (IsTileTileType(possibleTiles[selectedTile], RoadNoEdges))
                {
	                emptyTiles->Add(a_pos);
                }
                else if (i >= worldSize - 1 && IsTileTileType(possibleTiles[selectedTile], RoadEnd))
                {
	                endTiles->Add(a_pos);
                }

                //select a random tile out of the possible ones and spawns it
                tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y), possibleTiles[selectedTile].rotation);
                if (tileActor)
                {
                    tileActor->GetStaticMeshComponent()->SetStaticMesh(possibleTiles[selectedTile].tileMesh);
                    tileActor->SetMobility(EComponentMobility::Static);
                }

                currentGeneratedTiles.Add(PositionAndEdgeData(a_pos, possibleTiles[selectedTile].edgeHasRoad));
                lastGeneratedTiles.Add(PositionAndEdgeData(a_pos, possibleTiles[selectedTile].edgeHasRoad));

        
            }
        }
        else
        {
            TArray<FVector> worldPositions = NextPositionInWorldSpace(i);

            for (FVector a_pos : worldPositions)
            {
                
            	tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y), FRotator::ZeroRotator);
                if (tileActor)
                {
                    tileActor->GetStaticMeshComponent()->SetStaticMesh((*possibleTilesMap)[0][0][0].tileMesh);
                    tileActor->SetMobility(EComponentMobility::Static);
                }
                tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y), FRotator::ZeroRotator);
                if (tileActor)
                {
                    tileActor->GetStaticMeshComponent()->SetStaticMesh(worldBorder[randomNumber.RandRange(0, worldBorder.Num() - 1)]);
                    tileActor->SetMobility(EComponentMobility::Static);
                }
            }
        }
        lastGeneratedTiles = currentGeneratedTiles;
        currentGeneratedTiles.Empty();

	    
    }

    
    for (int i = emptyTiles->Num() - 1; i >= 0; i--)
    {
        int natureDensityRandom = randomNumber.RandRange(1, 100);
        FVector position = (*emptyTiles)[i];
        if (natureDensityRandom <= natureDensity)
        {
            tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(position.X, position.Y), FRotator::ZeroRotator);
            if (tileActor)
            {
                tileActor->GetStaticMeshComponent()->SetStaticMesh(natureDecorations[randomNumber.RandRange(0, natureDecorations.Num() - 1)]);
                tileActor->SetMobility(EComponentMobility::Static);
            }
            emptyTiles->RemoveAt(i);
        }
    }
}






//initializes the map of all possible tiles
void AOverworldGeneration::InitializeTMap()
{
	TArray<TileRoadWithRotationData> tempData = {};
    for (int i = 0; i <= 6; i++)
    { 
        possibleTilesMap->Add(i, TArray<TArray<TileRoadWithRotationData>>());
    }
	for (UOverworldTileData* a_tileData : tileData)
	{
        UE_LOG(LogTemp, Log, TEXT("Der spaß wird ausgeführt \n"));
        int roadCount = 0;
        tempData = GetRotatedTileAndEdges(a_tileData,roadCount);
        possibleTilesMap->Find(roadCount)->Add(tempData);
	}
    
}

//calculates all possible placements of a tile
TArray<AOverworldGeneration::TileRoadWithRotationData> AOverworldGeneration::GetRotatedTileAndEdges(UOverworldTileData* a_tileData, int& a_roadCount)
{
	TArray<TileRoadWithRotationData> shiftedEdgeDataAndRotation;
    TArray<bool> edgeData = a_tileData->GetEdgeData();

    FRotator rotations;
    TArray<bool> shiftedEdgeData;

    a_roadCount = 0;

    for (int i = 0; i < edgeData.Num(); i++)
    {
    	rotations = FRotator(0, i * 60, 0);
        shiftedEdgeData = {false,false,false,false,false,false};

        bool allowToAdd = true;

        for (int j = 0; j < edgeData.Num(); j++)
        {
	        shiftedEdgeData[(j + i) % edgeData.Num()] = edgeData[j];
        }
	    for (TileRoadWithRotationData a_data : shiftedEdgeDataAndRotation)
	    {
		    if (shiftedEdgeData == a_data.edgeHasRoad)
		    {
			    allowToAdd = false;
                break;
		    }
	    }
        if (allowToAdd)
        {
	        shiftedEdgeDataAndRotation.Add(TileRoadWithRotationData(a_tileData->GetTileMeshPtr(),rotations, shiftedEdgeData));
        }
    }

    for (int i = 0; i < edgeData.Num(); i++)
    {
	    if (edgeData[i] == true)
	    {
		    a_roadCount++;
	    }
    }

    return shiftedEdgeDataAndRotation;
}

//calculates the actual placeing position of a tile based on the raw position
FVector AOverworldGeneration::TilePosition(int a_width, int a_height)
{
    float y = tileSize * (FMath::Sqrt(12.0f) / 2 * a_height);
    float x = -1 * tileSize * (2 * a_width + a_height % 2);
    return FVector(x, y, 0);
}

//calculates the next raw position in a grid
TArray<FVector> AOverworldGeneration::NextPositionInWorldSpace(int a_circleIteration)
{
    TArray<FVector> nextPositions;

    if (a_circleIteration == 0)
    {
	    nextPositions.Add(FVector(0,0,0));
        return nextPositions;
    }

    for (int y = a_circleIteration; y >= (a_circleIteration * (-1)); y--)
    {
        if (FMath::Abs(y) == a_circleIteration)
        {
            if (y % 2 == 0)
            {
                for (int x = (a_circleIteration - ((a_circleIteration + 1) / 2)) * (-1); x <= (a_circleIteration - ((a_circleIteration + 1) / 2 - 1)) - 1; x++)
                {
                    nextPositions.Add(FVector(x, y, 0));
                }
            }
            else
            {
                if (y < 0)
                {
                    for (int x = (a_circleIteration - ((a_circleIteration + 1) / 2)) * (-1); x <= (a_circleIteration - ((a_circleIteration + 1) / 2 - 1)); x++)
                    {
                        nextPositions.Add(FVector(x, y, 0));
                    }
                }
                if (y > 0)
                {
                    for (int x = (a_circleIteration - ((a_circleIteration + 1) / 2 - 1)) * (-1); x <= (a_circleIteration - ((a_circleIteration + 1) / 2 - 1)) - 1; x++)
                    {
                        nextPositions.Add(FVector(x, y, 0));
                    }
                }
            }
        }
        else if (y % 2 == 0)
        {
            if (y == 0)
            {
                nextPositions.Add(FVector(a_circleIteration * (-1), y, 0));
                nextPositions.Add(FVector(a_circleIteration, y, 0));
            }
            if (y < 0)
            {
                nextPositions.Add(FVector((a_circleIteration - y * (-1) / 2) * (-1), y, 0));
                nextPositions.Add(FVector((a_circleIteration - y * (-1) / 2), y, 0));
            }
            if (y > 0)
            {
                nextPositions.Add(FVector((a_circleIteration - y / 2) * (-1), y, 0));
                nextPositions.Add(FVector((a_circleIteration - y / 2), y, 0));
            }
        }
        else
        {
            if (y < 0)
            {
                nextPositions.Add(FVector(((a_circleIteration - y * (-1) / 2) - 1) * (-1), y, 0));
                nextPositions.Add(FVector((a_circleIteration - y * (-1) / 2), y, 0));
            }
            if (y > 0)
            {
                nextPositions.Add(FVector((a_circleIteration - y / 2) * (-1), y, 0));
                nextPositions.Add(FVector((a_circleIteration - y / 2) - 1, y, 0));
            }
        }
    }

    return nextPositions;
}

//returns true if that tiles edge need a road, false if its not allowed to have a road, unset if it doesnt matter
TOptional<bool> AOverworldGeneration::GetNeighboringTilesEdgeData(FVector a_currentPos, FVector a_offset, int a_edgeToCheck)
{
    TOptional<bool> edgeData;
    edgeData.Reset();

    bool posFound = false;

    for (PositionAndEdgeData a_tile : lastGeneratedTiles)
    {
	    if (a_tile.position == a_currentPos + a_offset)
	    {
		    posFound = true;
	    }

        if (posFound == true)
        {
	        edgeData = a_tile.edgeHasRoad[(a_edgeToCheck + 3) % 6];
            posFound = false;
            break;
        }
    }

    return edgeData;
}

//returns the the bools for all edges of the object that is next generated
TArray<TOptional<bool>> AOverworldGeneration::GetNeededEdgeData(FVector a_currentPos, TArray<FVector> a_offsets)
{
	TArray<TOptional<bool>> edgeData;
    edgeData.SetNum(6);

    for (int i = 0; i < a_offsets.Num(); i++)
    {
	    edgeData[i] = GetNeighboringTilesEdgeData(a_currentPos, a_offsets[i], i);
    }

    return edgeData;
}

//outputs the minimum needed edges with roads and the maximum allowed edges with a road
void AOverworldGeneration::GetPossibleRoadCount(TArray<TOptional<bool>> a_neededEdgeData, int& a_minRoadCount, int& a_maxRoadCount)
{
	a_minRoadCount = 0;
    a_maxRoadCount = 0;
    for (int i = 0; i < a_neededEdgeData.Num(); i++)
    {
            
	    if (a_neededEdgeData[i] == true)
	    {
		    a_minRoadCount++;
            a_maxRoadCount++;
	    }
        else if (!a_neededEdgeData[i].IsSet())
        {
	        a_maxRoadCount++;
        }
    }
}


bool AOverworldGeneration::IsTileTileType(TileRoadWithRotationData a_tile, TileType a_tiletype)
{
    TArray<bool> tilesBaseBool = {};

	switch (a_tiletype)
	{
	case RoadNoEdges:
        tilesBaseBool = {false,false,false,false,false,false};

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

	        for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
	        {
		        if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
		        {
			        match = false;
                    break;
		        }
	        }

            if (match == true)
            {
	            return true;
            }
        }
        return false;

    case RoadEnd:
        tilesBaseBool = { false,true,false,false,false,false };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadSmallCurve:
        tilesBaseBool = { false,true,true,false,false,false };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadBigCurve:
        tilesBaseBool = { false,true,false,true,false,false };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadStraight:
        tilesBaseBool = { false,true,false,false,true,false };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadE:
        tilesBaseBool = { true,true,true,false,false,false };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadYLeft:
        tilesBaseBool = { false,true,false,true,true,false };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadYRight:
        tilesBaseBool = { false,true,false,false,true,true };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadYWide:
        tilesBaseBool = { false,true,false,true,false,true };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadX:
        tilesBaseBool = { true,false,true,true,false,true };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadK:
        tilesBaseBool = { true,true,false,false,true,true };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadYPlusOne:
        tilesBaseBool = { true,true,true,false,true,false };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadFiveEdges:
        tilesBaseBool = { true,true,true,true,false,true };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;

    case RoadAllEdges:
        tilesBaseBool = { true,true,true,true,true,true };

        for (int i = 0; i < a_tile.edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.edgeHasRoad[(j + i) % 6])
                {
                    match = false;
                    break;
                }
            }

            if (match == true)
            {
                return true;
            }
        }
        return false;
	default:
        return false;
    }
}
