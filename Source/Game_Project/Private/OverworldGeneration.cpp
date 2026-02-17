// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldGeneration.h"

#include "MultiplayerGameState.h"
#include "Engine/StaticMeshActor.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOverworldGeneration::AOverworldGeneration()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    m_worldGenerated = false;
}

// Called when the game starts or when spawned
void AOverworldGeneration::BeginPlay()
{
	Super::BeginPlay();

    AMultiplayerGameState* gs = GetWorld()->GetGameState<AMultiplayerGameState>();

    if (!gs)
    {
	    return;
    }

    if (gs->m_overworldSeed != 0)
    {
	    HandleSeedReady(gs->m_overworldSeed);
    }
    else
    {
	    gs->OnOverworldSeedReady.AddDynamic(this, &AOverworldGeneration::HandleSeedReady);
    }
}

void AOverworldGeneration::HandleSeedReady(int32 a_seed)
{
	if (m_worldGenerated)
	{
		return;
	}

    m_worldGenerated = true;

    GenerateWorld(a_seed);
}


void AOverworldGeneration::GenerateWorld(int32 a_seed)
{
    UE_LOG(LogTemp, Warning, TEXT("Generating World with Seed: %d on %s"), a_seed, HasAuthority() ? TEXT("Host") : TEXT("Client"));

    m_chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(),ACustomChunkManager::StaticClass()));

    //if (m_seed == 0)
    //{
    //    m_seed = FMath::Rand();
    //    UE_LOG(LogTemp, Log, TEXT("Random Seed: %d"), m_seed);
    //}
    m_randomNumber.Initialize(a_seed);

    InitializeTMap();

    AStaticMeshActor* tileActor = nullptr;

    TArray<FVector> offset = {};

    GetWorld()->SpawnActor<AActor>(m_musicActor, FVector::ZeroVector + m_worldOffsetVector, FRotator::ZeroRotator);

    for (int i = 0; i <= m_worldSize + 10; i++)
    {
        if (i == 0)
        {
            //generate starting tile
            int roadCountIndex = m_randomNumber.RandRange(m_startRoadCountMin, m_startRoadCountMax);
            int tileIndex = m_randomNumber.RandRange(0, (*m_possibleTilesMap)[roadCountIndex].Num() - 1);
            int tileRotationIndex = m_randomNumber.RandRange(0, (*m_possibleTilesMap)[roadCountIndex][tileIndex].Num() - 1);
            //tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector + m_worldOffsetVector, (*m_possibleTilesMap)[roadCountIndex][tileIndex][tileRotationIndex].m_rotation);
            tileActor = Cast<AStaticMeshActor>(m_chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), FVector::ZeroVector + m_worldOffsetVector, (*m_possibleTilesMap)[roadCountIndex][tileIndex][tileRotationIndex].m_rotation, FActorSpawnParameters()));
            if (tileActor)
            {
                tileActor->SetMobility(EComponentMobility::Movable);
                tileActor->GetStaticMeshComponent()->SetStaticMesh((*m_possibleTilesMap)[roadCountIndex][tileIndex][tileRotationIndex].m_tileMesh);
                tileActor->SetMobility(EComponentMobility::Static);
            }
            PositionAndEdgeData tempData = PositionAndEdgeData(FVector(0,0,0), (*m_possibleTilesMap)[roadCountIndex][tileIndex][tileRotationIndex].m_edgeHasRoad);
            m_lastGeneratedTiles.Add(tempData);
            m_currentGeneratedTiles.Add(tempData);
            
        }
        else if (i <= m_worldSize)
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
                if (i == m_worldSize - 1 && minRoadCount > 0)
                {
                    maxRoadCount = minRoadCount;
                }

                //possibility of road spreading //change to not spreading closely at world end
                if (minRoadCount <= 2 && maxRoadCount >= 2 && i < m_worldSize - 1)
                {
                    if (i < m_worldSize - m_noSpreadFromWorldBorder)
                    {
                        int roadSpreadRandom = m_randomNumber.RandRange(1, 100);
                        if (roadSpreadRandom >= m_roadSpreadPossability)
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
                    else
                    {
                        maxRoadCount = minRoadCount;
                    }
                }

                TArray<FTileRoadWithRotationData> possibleTiles;

                for (int j = minRoadCount; j <= maxRoadCount; j++)
                {
                    if (j == 1 && i <= m_worldSize - 2)//no single road tile until end of world
                    {
                        j++;
                    }
                    for(TArray<FTileRoadWithRotationData> a_tileArray : (*m_possibleTilesMap)[j])
                    {
                        for(FTileRoadWithRotationData a_tile : a_tileArray)//compare tiles bool array with neededEdgeData but ignoring the null data
                        {
                            for (int k = 0; k < a_tile.m_edgeHasRoad.Num(); k++)
                            {
                                if (!neededEdgeData[k].IsSet())
                                {
                                    
                                }
                                else if (neededEdgeData[k] != a_tile.m_edgeHasRoad[k])
                                {
                                    break;
                                }
                                if (k == a_tile.m_edgeHasRoad.Num() - 1)
                                {
                                    possibleTiles.Add(a_tile);
                                }
                            }
                        }
                    }
                }

                int selectedTile = 0;
                int roadStraightnessRandom = m_randomNumber.RandRange(1, 100);
                if (roadStraightnessRandom <= m_roadStraightness)
                {
                    selectedTile = 0;
                    TArray<int> bigCurveRoadTiles = {};
                    TArray<int> threeRoadTiles = {};
                    for (FTileRoadWithRotationData a_tile : possibleTiles)
                    {
                        
                        if (IsTileTileType(a_tile, RoadStraight))
                        {
                            break;
                        }
                        else if (IsTileTileType(a_tile, RoadBigCurve))
                        {
                            bigCurveRoadTiles.Add(selectedTile);
                        }
                        else if (IsTileTileType(a_tile, RoadE) || IsTileTileType(a_tile, RoadYLeft) || IsTileTileType(a_tile, RoadYRight) || IsTileTileType(a_tile, RoadYWide))
                        {
                            threeRoadTiles.Add(selectedTile);
                        }
                        selectedTile++;
                    }

                    if (selectedTile >= possibleTiles.Num())
                    {
                        if (bigCurveRoadTiles.Num() > 0)
                        {
                            selectedTile = bigCurveRoadTiles[m_randomNumber.RandRange(0, bigCurveRoadTiles.Num() - 1)];
                        }
                        else if (threeRoadTiles.Num() > 0)
                        {
                            selectedTile = threeRoadTiles[m_randomNumber.RandRange(0, threeRoadTiles.Num() - 1)];
                        }
                        else
                        {
                            selectedTile = m_randomNumber.RandRange(0, possibleTiles.Num() - 1);
                        }
                    }
                }
                else
                {
                    selectedTile = 0;
                    TArray<int> bigCurveRoadTiles = {};
                    TArray<int> threeRoadTiles = {};
                    for (FTileRoadWithRotationData a_tile : possibleTiles)
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
                        selectedTile = bigCurveRoadTiles[m_randomNumber.RandRange(0, bigCurveRoadTiles.Num() - 1)];
                    }
                    else if (threeRoadTiles.Num() > 0)
                    {
                        selectedTile = threeRoadTiles[m_randomNumber.RandRange(0, threeRoadTiles.Num() - 1)];
                    }
                    else
                    {
                        selectedTile = m_randomNumber.RandRange(0, possibleTiles.Num() - 1);
                    }
                }

                if (IsTileTileType(possibleTiles[selectedTile], RoadNoEdges))
                {
	                m_emptyTiles.Add(a_pos);
                }
                else if (i >= m_worldSize - 1 && IsTileTileType(possibleTiles[selectedTile], RoadEnd))
                {
	                m_endTiles.Add(a_pos);
                }

                //select a random tile out of the possible ones and spawns it
                //tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y) + m_worldOffsetVector, possibleTiles[selectedTile].m_rotation);
                tileActor = Cast<AStaticMeshActor>(m_chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y) + m_worldOffsetVector, possibleTiles[selectedTile].m_rotation, FActorSpawnParameters()));
                if (tileActor)
                {
                    tileActor->SetMobility(EComponentMobility::Movable);
                    tileActor->GetStaticMeshComponent()->SetStaticMesh(possibleTiles[selectedTile].m_tileMesh);
                    tileActor->SetMobility(EComponentMobility::Static);
                }

                m_currentGeneratedTiles.Add(PositionAndEdgeData(a_pos, possibleTiles[selectedTile].m_edgeHasRoad));
                m_lastGeneratedTiles.Add(PositionAndEdgeData(a_pos, possibleTiles[selectedTile].m_edgeHasRoad));

        
            }
        }
        else
        {
            TArray<FVector> worldPositions = NextPositionInWorldSpace(i);

            for (FVector a_pos : worldPositions)
            {
                
            	//tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y) + m_worldOffsetVector, FRotator::ZeroRotator);
                tileActor = Cast<AStaticMeshActor>(m_chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y) + m_worldOffsetVector, FRotator::ZeroRotator, FActorSpawnParameters()));
                if (tileActor)
                {
                    tileActor->SetMobility(EComponentMobility::Movable);
                    tileActor->GetStaticMeshComponent()->SetStaticMesh((*m_possibleTilesMap)[0][0][0].m_tileMesh);
                    tileActor->SetMobility(EComponentMobility::Static);
                }
                //tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y) + m_worldOffsetVector, FRotator::ZeroRotator);
                tileActor = Cast<AStaticMeshActor>(m_chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), TilePosition(a_pos.X, a_pos.Y) + m_worldOffsetVector, FRotator::ZeroRotator, FActorSpawnParameters()));
            	if (tileActor)
                {
                    tileActor->SetMobility(EComponentMobility::Movable);
                    tileActor->GetStaticMeshComponent()->SetStaticMesh(m_worldBorder[m_randomNumber.RandRange(0, m_worldBorder.Num() - 1)]);
                    tileActor->SetMobility(EComponentMobility::Static);
                }
            }
        }
        m_lastGeneratedTiles = m_currentGeneratedTiles;
        m_currentGeneratedTiles.Empty();

	    
    }

    //nature placement
    for (int i = m_emptyTiles.Num() - 1; i >= 0; i--)
    {
        int natureDensityRandom = m_randomNumber.RandRange(1, 100);
        FVector position = m_emptyTiles[i];
        if (natureDensityRandom <= m_natureDensity)
        {
            //tileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TilePosition(position.X, position.Y) + m_worldOffsetVector, FRotator::ZeroRotator);
            tileActor = Cast<AStaticMeshActor>(m_chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), TilePosition(position.X, position.Y) + m_worldOffsetVector, FRotator::ZeroRotator, FActorSpawnParameters()));
            if (tileActor)
            {
                tileActor->SetMobility(EComponentMobility::Movable);
                tileActor->GetStaticMeshComponent()->SetStaticMesh(m_natureDecorations[m_randomNumber.RandRange(0, m_natureDecorations.Num() - 1)]);
                tileActor->SetMobility(EComponentMobility::Static);
            }
            m_emptyTiles.RemoveAt(i);
        }
    }

    //dungeon portals spawn
	for (int i = m_endTiles.Num() - 1; i >= 0; i--)
	{
		FVector position = m_endTiles[i];
        ADungeonEntrancePortal* portalActor = nullptr;
		//portalActor = GetWorld()->SpawnActor<ADungeonEntrancePortal>(m_dungeonEntrance, TilePosition(position.X, position.Y) + FVector(0, 0, m_dungeonEntranceHeightOffset) + m_worldOffsetVector, FRotator::ZeroRotator);
        portalActor = Cast<ADungeonEntrancePortal>(m_chunkManager->SpawnActorInChunk(m_dungeonEntrance, TilePosition(position.X, position.Y) + FVector(0, 0, m_dungeonEntranceHeightOffset) + m_worldOffsetVector, FRotator::ZeroRotator, FActorSpawnParameters()));
	}

    //enemiesSpawn
    if (HasAuthority())
    {
        for (int i = m_emptyTiles.Num() - 1; i >= 0; i--)
        {
            int enemyDensityRandom = m_randomNumber.RandRange(1, 100);
            FVector position = m_emptyTiles[i];
            float currentDistToSpawn = FVector::Dist(TilePosition(position.X, position.Y) + m_worldOffsetVector, FVector::ZeroVector + m_worldOffsetVector);

            //UE_LOG(LogTemp,Error, TEXT("currentDistance: %f"), currentDistToSpawn)

            if (currentDistToSpawn >= m_enemyFreeRangeFromSpawn)
            {
                if (enemyDensityRandom <= m_enemyDensity)
                {
                    AEnemyCharacter* enemy;
                    enemy = Cast<AEnemyCharacter>(m_chunkManager->SpawnActorInChunk(m_possibleEnemies[m_randomNumber.RandRange(0, m_possibleEnemies.Num() - 1)], TilePosition(position.X, position.Y) + m_worldOffsetVector, FRotator::ZeroRotator, FActorSpawnParameters()));

                    m_emptyTiles.RemoveAt(i);
                }
            }
        }
    }

    //torch placement
    for (int i = m_emptyTiles.Num() - 1; i >= 0; i--)
    {
        int torchDensityRandom = m_randomNumber.RandRange(1, 100);
        FVector position = m_emptyTiles[i];
        if (torchDensityRandom <= m_torchDensity)
        {
            AActor* actor;
            actor = Cast<AActor>(m_chunkManager->SpawnActorInChunk(m_torch, TilePosition(position.X, position.Y) + m_worldOffsetVector, FRotator::ZeroRotator, FActorSpawnParameters()));
            
            m_emptyTiles.RemoveAt(i);
        }
    }



    if (!m_navMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("NavMesh not Found"));
    }
    else
    {
	    m_navMesh->SetActorLocation(FVector(0,0,0) + m_worldOffsetVector);
		m_navMesh->SetActorScale3D(FVector(m_worldSize * 2, m_worldSize * 2,0.2));
        if (UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
        {
            navSys->Build();
        }
    }

    delete m_possibleTilesMap;
    m_possibleTilesMap = nullptr;
}






//initializes the map of all possible tiles
void AOverworldGeneration::InitializeTMap()
{
	TArray<FTileRoadWithRotationData> tempData = {};
    for (int i = 0; i <= 6; i++)
    { 
        m_possibleTilesMap->Add(i, TArray<TArray<FTileRoadWithRotationData>>());
    }
	for (UOverworldTileData* a_tileData : m_tileData)
	{
        int roadCount = 0;
        tempData = GetRotatedTileAndEdges(a_tileData,roadCount);
        m_possibleTilesMap->Find(roadCount)->Add(tempData);
	}
    
}

//calculates all possible placements of a tile
TArray<FTileRoadWithRotationData> AOverworldGeneration::GetRotatedTileAndEdges(UOverworldTileData* a_tileData, int& a_roadCount)
{
	TArray<FTileRoadWithRotationData> shiftedEdgeDataAndRotation;
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
	    for (FTileRoadWithRotationData a_data : shiftedEdgeDataAndRotation)
	    {
		    if (shiftedEdgeData == a_data.m_edgeHasRoad)
		    {
			    allowToAdd = false;
                break;
		    }
	    }
        if (allowToAdd)
        {
	        shiftedEdgeDataAndRotation.Add(FTileRoadWithRotationData(a_tileData->GetTileMeshPtr(),rotations, shiftedEdgeData));
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

//calculates the actual placeing m_position of a tile based on the raw m_position
FVector AOverworldGeneration::TilePosition(int a_width, int a_height)
{
    float y = m_tileSize * (FMath::Sqrt(12.0f) / 2 * a_height);
    float x = -1 * m_tileSize * (2 * a_width + a_height % 2);
    return FVector(x, y, 0);
}

//calculates the next raw m_position in a grid
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

    for (PositionAndEdgeData a_tile : m_lastGeneratedTiles)
    {
	    if (a_tile.m_position == a_currentPos + a_offset)
	    {
		    posFound = true;
	    }

        if (posFound == true)
        {
	        edgeData = a_tile.m_edgeHasRoad[(a_edgeToCheck + 3) % 6];
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


bool AOverworldGeneration::IsTileTileType(FTileRoadWithRotationData a_tile, TileType a_tiletype)
{
    TArray<bool> tilesBaseBool = {};

	switch (a_tiletype)
	{
	case RoadNoEdges:
        tilesBaseBool = {false,false,false,false,false,false};

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

	        for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
	        {
		        if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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

        for (int i = 0; i < a_tile.m_edgeHasRoad.Num(); i++)
        {
            bool match = true;

            for (int j = 0; j < a_tile.m_edgeHasRoad.Num(); j++)
            {
                if (tilesBaseBool[j] != a_tile.m_edgeHasRoad[(j + i) % 6])
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
