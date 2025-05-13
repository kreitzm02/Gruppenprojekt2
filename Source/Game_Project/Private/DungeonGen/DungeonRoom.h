// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonRoom.generated.h"

UENUM()
enum class ERoomType : uint8
{
	EMPTY, DECORATION, LOOT, START, END, BOSS
};

USTRUCT()
struct FDungeonRoom
{
	GENERATED_BODY()

	FInt32Vector2 m_RoomOrigin;
	int32 m_RoomCellWidth;
	int32 m_RoomCellLength;
	ERoomType m_RoomType;

	FDungeonRoom() : m_RoomOrigin(0, 0), m_RoomCellWidth(0), m_RoomCellLength(0), m_RoomType(ERoomType::EMPTY) {}
	FDungeonRoom(const FInt32Vector2& a_RoomOrigin, int32 a_LengthX, int32 a_WidthY) 
		: m_RoomOrigin(a_RoomOrigin), m_RoomCellLength(a_LengthX), m_RoomCellWidth(a_WidthY) {}

	FInt32Vector2 GetRoomCenter() const;
	bool OverlapsWith(const FDungeonRoom& a_Other) const;
	void SetRoomType(const ERoomType& a_RoomType);
};