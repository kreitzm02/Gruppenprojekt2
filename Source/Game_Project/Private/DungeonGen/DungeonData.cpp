// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonData.h"

FDungeonData::FDungeonData(int a_RoomAmount, int a_GridLength, int a_GridWidth)
{
	m_AllRooms.SetNum(a_RoomAmount);
	m_AllRooms.Empty();
	m_AllRoomCenters.SetNum(a_RoomAmount);
	m_AllRoomCenters.Empty();
	m_DungeonGrid.SetNum(a_GridLength);
	for (int i = 0; i < a_GridLength; i++)
	{
		m_DungeonGrid[i].SetNum(a_GridWidth);
	}
}

void FDungeonData::GetStartAndEndRoom(const TPair<FInt32Vector2, FInt32Vector2>& a_Positions)
{
	for (int i = 0; i < m_AllRooms.Num(); i++)
	{
		if (m_AllRooms[i].GetRoomCenter() == a_Positions.Key)
		{
			m_AllRooms[i].SetRoomType(ERoomType::START);
			m_StartRoom = m_AllRooms[i];
		}
		else if (m_AllRooms[i].GetRoomCenter() == a_Positions.Value)
		{
			m_AllRooms[i].SetRoomType(ERoomType::END);
			m_EndRoom = m_AllRooms[i];
		}
	}
}

void FDungeonData::GetDeadEndRooms()
{
	for (int i = 0; i < m_AllRooms.Num(); i++)
	{
		if (m_DungeonAdjacencyList[m_AllRooms[i].GetRoomCenter()].Num() == 1)
		{
			if (m_AllRooms[i].m_RoomType == ERoomType::END || m_AllRooms[i].m_RoomType == ERoomType::START) continue;
			m_DeadEndRooms.Add(m_AllRooms[i]);
		}
	}
}
