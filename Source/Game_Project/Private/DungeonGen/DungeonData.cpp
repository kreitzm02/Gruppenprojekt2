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
			m_AllRooms[i].SetRoomType(ERoomType::ENTRANCE);
			m_StartRoom = m_AllRooms[i];
			UE_LOG(LogTemp, Log, TEXT("Start Room: %d, %d"),
				m_StartRoom.GetRoomCenter().X, m_StartRoom.GetRoomCenter().Y);
		}
		else if (m_AllRooms[i].GetRoomCenter() == a_Positions.Value)
		{
			m_AllRooms[i].SetRoomType(ERoomType::EXIT);
			m_EndRoom = m_AllRooms[i];
			UE_LOG(LogTemp, Log, TEXT("End Room: %d, %d"),
				m_EndRoom.GetRoomCenter().X, m_StartRoom.GetRoomCenter().Y);
		}
	}
}

void FDungeonData::GetDeadEndRooms()
{
	for (int i = 0; i < m_AllRooms.Num(); i++)
	{
		if (m_DungeonAdjacencyList[m_AllRooms[i].GetRoomCenter()].Num() == 1)
		{
			if (m_AllRooms[i].m_RoomType == ERoomType::EXIT || m_AllRooms[i].m_RoomType == ERoomType::ENTRANCE) continue;
			m_DeadEndRooms.Add(m_AllRooms[i]);
			m_AllRooms[i].m_RoomType = ERoomType::REWARD;
		}
	}
}
