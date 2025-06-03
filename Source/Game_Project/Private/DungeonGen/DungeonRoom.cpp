// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonRoom.h"

FInt32Vector2 FDungeonRoom::GetRoomCenter() const
{
	FInt32Vector2 result = FInt32Vector2(m_RoomOrigin.X + m_RoomCellLength / 2, m_RoomOrigin.Y + m_RoomCellWidth / 2);
	return result;
}

bool FDungeonRoom::OverlapsWith(const FDungeonRoom& a_Other) const
{
	return !(m_RoomOrigin.X + m_RoomCellLength < a_Other.m_RoomOrigin.X || a_Other.m_RoomOrigin.X + a_Other.m_RoomCellLength < m_RoomOrigin.X ||
		m_RoomOrigin.Y + m_RoomCellWidth < a_Other.m_RoomOrigin.Y || a_Other.m_RoomOrigin.Y + a_Other.m_RoomCellWidth < m_RoomOrigin.Y);
}

void FDungeonRoom::SetRoomType(const ERoomType& a_RoomType)
{
	m_RoomType = a_RoomType;
}

void FDungeonRoom::SetRoomType()
{
	if (m_RoomCellWidth > 6 && m_RoomCellLength > 6)
	{
		m_RoomType = (ERoomType)FMath::RandRange(0, 3);
	}
	else
	{
		m_RoomType = ERoomType::EMPTY;
	}
}

