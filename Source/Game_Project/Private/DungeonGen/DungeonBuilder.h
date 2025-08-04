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

	UPROPERTY()
	UDungeonTheme* m_DungeonTheme;

	FDungeonData* m_Data;

	UPROPERTY()
	UWorld* m_WorldContext;

	float m_WallOffset;

	UPROPERTY()
	ULevel* m_Level;

public:
	void Init(float a_UnitSize, UDungeonTheme* a_Theme, FDungeonData* a_Data, UWorld* a_World, float a_WallOffset, ULevel* a_Level);

	UFUNCTION()
	void BuildFloor();

	UFUNCTION()
	void BuildWall();

	UFUNCTION()
	void BuildDebugObjects();

	UFUNCTION()
	void BuildDecorationObjects();

	UFUNCTION()
	void BuildTorches();

	UFUNCTION()
	void BuildBossRoom();

	UFUNCTION()
	void GenerateEnemies();

	UFUNCTION()
	void SpawnBossEnemyRandom();

private:
	UFUNCTION()
	void TryPlaceWall(int32 a_GridX, int32 a_GridY, const FVector& a_Position, const FRotator& a_Rotation, int32 a_WallIndex) const;

	UFUNCTION()
	void TryPlaceDoor(int32 a_GridX, int32 a_GridY) const;

	UFUNCTION()
	bool IsWithinBounds(int32 a_GridX, int32 a_GridY) const;

	bool TryPlacePrefabCornerFacingCenter(TArray<TArray<ECellType>> a_Grid, int32 a_GridX, int32 a_GridY, FDungeonRoom a_Room, UStaticMesh* a_Mesh, int32 a_Probabilty);

	bool TryPlacePrefabCornerOrthoRotation(TArray<TArray<ECellType>> a_Grid, int32 a_GridX, int32 a_GridY, FDungeonRoom a_Room, UStaticMesh* a_Mesh, int32 a_Probabilty);

	UFUNCTION()
	void PlaceTorchAt(int32 a_X, int32 a_Y, const FVector& a_PositionOffset, const FRotator& a_RotationOffset);
};
