// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonRoom.h"
#include "DungeonGridUtils.h"
#include "DungeonGenUtils.generated.h"

UCLASS()
class UDungeonGenUtils : public UObject
{
	GENERATED_BODY()
	
public:
	static FInt32Vector2 GetRandomRoomSize(int32 a_Min, int32 a_Max, FRandomStream& a_Stream);
	static FInt32Vector2 GetRandomRoomOrigin(int32 a_RoomLengthX, int32 a_RoomWidthY, int32 a_GridLengthX, int32 a_GridWidthY, FRandomStream& a_Stream);
	static TMap<FInt32Vector2, TArray<FInt32Vector2>> BuildAdjacencyList(const TArray<FInt32Vector2>& a_RoomCenters, const TArray<TPair<FInt32Vector2, FInt32Vector2>>& a_MST);
	static TPair<FInt32Vector2, FInt32Vector2> DetermineDungeonDiameter(const TArray<FInt32Vector2>& a_RoomCenters, const TMap<FInt32Vector2, TArray<FInt32Vector2>>& a_AdjacencyList);
	static FInt32Vector2 BFSFindFarthestNode(const FInt32Vector2& a_Start, const TArray<FInt32Vector2>& a_RoomCenters, const TMap<FInt32Vector2, TArray<FInt32Vector2>>& a_AdjacencyList);
	static TArray<TPair<FInt32Vector2, FInt32Vector2>> CreateMST(const TArray<FInt32Vector2>& a_RoomCenters, const TArray<TPair<TPair<FInt32Vector2, FInt32Vector2>, float>>& a_Connections);
	static TArray<FInt32Vector2> GetCellsToModifyFromVariant(const FDungeonRoom& a_Room);
	static bool CellIsNeighbourOfPosition(int32 a_GridX, int32 a_GridY, ECellType a_Cell, TArray<TArray<ECellType>> a_Grid, bool a_DiagonalIncluded = true);
	static int32 GetOrthogonalRotationBasedOnCenter(FVector a_Pos, FVector a_Center);
		
	// TODO - Noch zu migrierende Methoden: CellIsNeighbourOfPosition || GetRandomRotation || GetOrthoRotationBasedOnCenter
};
