// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonGridUtils.generated.h"

UENUM()
enum class ECellType : uint8
{
	EMPTY, FLOOR, WALL, WALLCORNER, WALLDEC, FLOORCORRIDOR, FLOORWITHDOOR, EMPTYFORCED
};

UCLASS()
class UDungeonGridUtils : public UObject
{
	GENERATED_BODY()
	
public:
	static TArray<FInt32Vector2> CollectAffectedCells(const FInt32Vector2& a_Origin, const FInt32Vector2& a_Size);
	static void ChangeCellsInGrid(const TArray<FInt32Vector2>& a_Cells, const ECellType& a_CellType, TArray<TArray<ECellType>>& a_GridToChange);
	static void ChangeCellsInGridForced(const TArray<FInt32Vector2>& a_Cells, const ECellType& a_CellType, TArray<TArray<ECellType>>& a_GridToChange);
};
