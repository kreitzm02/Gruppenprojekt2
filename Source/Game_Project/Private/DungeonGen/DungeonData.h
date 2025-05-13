// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonRoom.h"
#include "DungeonGridUtils.h"
#include "DungeonData.generated.h"

USTRUCT()
struct FDungeonData
{
	GENERATED_BODY()

	TArray<FDungeonRoom> m_AllRooms;
	TArray<FInt32Vector2> m_AllRoomCenters;
	FDungeonRoom m_StartRoom;
	FDungeonRoom m_EndRoom;
	TArray<FDungeonRoom> m_DeadEndRooms;
	TArray<TPair<FInt32Vector2, FInt32Vector2>> m_DungeonMST;
	TMap<FInt32Vector2, TArray<FInt32Vector2>> m_DungeonAdjacencyList;
	TArray<TArray<ECellType>> m_DungeonGrid;
};
