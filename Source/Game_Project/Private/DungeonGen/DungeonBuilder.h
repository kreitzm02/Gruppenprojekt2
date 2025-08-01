// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonData.h"
#include "DungeonTheme.h"
#include "DungeonBuilder.generated.h"

/**
 * 
 */
UCLASS()
class UDungeonBuilder : public UObject
{
	GENERATED_BODY()

	float m_UnitSize;
	UDungeonTheme* m_DungeonTheme;
	FDungeonData* m_Data;
	UWorld* m_WorldContext;
	float m_WallOffset;
	ULevel* m_Level;

public:
	void Init(float a_UnitSize, UDungeonTheme* a_Theme, FDungeonData* a_Data, UWorld* a_World, float a_WallOffset, ULevel* a_Level);
	void BuildFloor();
	void BuildWall();
	void BuildDebugObjects();
	void BuildDecorationObjects();
	void BuildTorches();
	void BuildBossRoom();
	void GenerateEnemies();
	void SpawnBossEnemyRandom();

private:
	void TryPlaceWall(int32 a_GridX, int32 a_GridY, const FVector& a_Position, const FRotator& a_Rotation, int32 a_WallIndex) const;
	void TryPlaceDoor(int32 a_GridX, int32 a_GridY) const;
	bool IsWithinBounds(int32 a_GridX, int32 a_GridY) const;
	bool TryPlacePrefabCornerFacingCenter(TArray<TArray<ECellType>> a_Grid, int32 a_GridX, int32 a_GridY, FDungeonRoom a_Room, UStaticMesh* a_Mesh, int32 a_Probabilty);
	bool TryPlacePrefabCornerOrthoRotation(TArray<TArray<ECellType>> a_Grid, int32 a_GridX, int32 a_GridY, FDungeonRoom a_Room, UStaticMesh* a_Mesh, int32 a_Probabilty);
	void PlaceTorchAt(int32 a_X, int32 a_Y, const FVector& a_PositionOffset, const FRotator& a_RotationOffset);
};
