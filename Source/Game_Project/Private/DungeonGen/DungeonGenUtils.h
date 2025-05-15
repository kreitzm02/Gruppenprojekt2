// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonRoom.h"
#include "DungeonGenUtils.generated.h"

UCLASS()
class UDungeonGenUtils : public UObject
{
	GENERATED_BODY()
	
public:
	static FInt32Vector2 GetRandomRoomSize(int32 a_Min, int32 a_Max);
	static FInt32Vector2 GetRandomRoomOrigin(int32 a_RoomLengthX, int32 a_RoomWidthY, int32 a_GridLengthX, int32 a_GridWidthY);
	static TMap<FInt32Vector2, TArray<FInt32Vector2>> BuildAdjacencyList(const TArray<FInt32Vector2>& a_RoomCenters, const TArray<TPair<FInt32Vector2, FInt32Vector2>>& a_MST);
	static TPair<FInt32Vector2, FInt32Vector2> DetermineDungeonDiameter(const TArray<FInt32Vector2>& a_RoomCenters, const TMap<FInt32Vector2, TArray<FInt32Vector2>>& a_AdjacencyList);
	static FInt32Vector2 BFSFindFarthestNode(const FInt32Vector2& a_Start, const TArray<FInt32Vector2>& a_RoomCenters, const TMap<FInt32Vector2, TArray<FInt32Vector2>>& a_AdjacencyList);
	static TArray<TPair<FInt32Vector2, FInt32Vector2>> CreateMST(const TArray<FInt32Vector2>& a_RoomCenters, const TArray<TPair<TPair<FInt32Vector2, FInt32Vector2>, float>>& a_Connections);
		
	// TODO - Noch zu migrierende Methoden: CellIsNeighbourOfPosition || GetRandomRotation || GetOrthoRotationBasedOnCenter
};
