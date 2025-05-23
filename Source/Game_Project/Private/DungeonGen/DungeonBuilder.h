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

public:
	void Init(float a_UnitSize, UDungeonTheme* a_Theme, FDungeonData* a_Data, UWorld* a_World, float a_WallOffset);
	void BuildFloor();
	void BuildWall();


private:
	void TryPlaceWall(int32 a_GridX, int32 a_GridY, const FVector& a_Position, const FRotator& a_Rotation, int32 a_WallIndex) const;
	bool IsWithinBounds(int32 a_GridX, int32 a_GridY) const;
};
