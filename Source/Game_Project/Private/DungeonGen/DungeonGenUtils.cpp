// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonGenUtils.h"

FInt32Vector2 UDungeonGenUtils::GetRandomRoomSize(int32 a_Min, int32 a_Max, FRandomStream& a_Stream)
{
	return FInt32Vector2(a_Stream.RandRange(a_Min, a_Max) , a_Stream.RandRange(a_Min, a_Max));
}

FInt32Vector2 UDungeonGenUtils::GetRandomRoomOrigin(int32 a_RoomLengthX, int32 a_RoomWidthY, int32 a_GridLengthX, int32 a_GridWidthY, FRandomStream& a_Stream)
{
	return FInt32Vector2(a_Stream.RandRange(1, a_GridLengthX - a_RoomLengthX - 1), a_Stream.RandRange(1, a_GridWidthY - a_RoomWidthY - 1));
}

// this will create a dictionary which will get each room their corresponding neighbour rooms ( room centers are used for this )
TMap<FInt32Vector2, TArray<FInt32Vector2>> UDungeonGenUtils::BuildAdjacencyList(const TArray<FInt32Vector2>& a_RoomCenters, const TArray<TPair<FInt32Vector2, FInt32Vector2>>& a_MST)
{
	TMap<FInt32Vector2, TArray<FInt32Vector2>> adjacencyList = {};
	for (int i = 0; i < a_RoomCenters.Num(); i++)
	{
		adjacencyList.Add(a_RoomCenters[i], TArray<FInt32Vector2>());
		//adjacencyList[a_RoomCenters[i]] = TArray<FInt32Vector2>();
	}
	
	for (int i = 0; i < a_MST.Num(); i++)
	{
		adjacencyList[a_MST[i].Key].Add(a_MST[i].Value);
		adjacencyList[a_MST[i].Value].Add(a_MST[i].Key);
	}

	return adjacencyList;
}

TPair<FInt32Vector2, FInt32Vector2> UDungeonGenUtils::DetermineDungeonDiameter(const TArray<FInt32Vector2>& a_RoomCenters, const TMap<FInt32Vector2, TArray<FInt32Vector2>>& a_AdjacencyList)
{
	FInt32Vector2 arbitraryNode = *a_RoomCenters.begin();
	FInt32Vector2 nodeA = BFSFindFarthestNode(arbitraryNode, a_RoomCenters, a_AdjacencyList);
	FInt32Vector2 nodeB = BFSFindFarthestNode(nodeA, a_RoomCenters, a_AdjacencyList);

	return TPair<FInt32Vector2, FInt32Vector2>(nodeA, nodeB);
}

FInt32Vector2 UDungeonGenUtils::BFSFindFarthestNode(const FInt32Vector2& a_Start, const TArray<FInt32Vector2>& a_RoomCenters, const TMap<FInt32Vector2, TArray<FInt32Vector2>>& a_AdjacencyList)
{
	TQueue<FInt32Vector2> queue = {};
	TMap<FInt32Vector2, int32> distances = {};

	for (int i = 0; i < a_RoomCenters.Num(); i++)
	{
		distances.Add(a_RoomCenters[i], -1);
		//distances[a_RoomCenters[i]] = -1;
	}

	distances[a_Start] = 0;
	queue.Enqueue(a_Start);

	FInt32Vector2 farthestNode = a_Start;
	int32 maxDistance = 0;

	while (!queue.IsEmpty())
	{
		FInt32Vector2 current;
		queue.Dequeue(current);
		int32 currentDist = distances[current];

		if (currentDist > maxDistance)
		{
			maxDistance = currentDist;
			farthestNode = current;
		}

		for (int i = 0; i < a_AdjacencyList[current].Num(); i++)
		{
			if (distances[a_AdjacencyList[current][i]] == -1)
			{
				distances[a_AdjacencyList[current][i]] = currentDist + 1;
				queue.Enqueue(a_AdjacencyList[current][i]);
			}
		}
	}
	return farthestNode;
}

// This method creates a mst based on kruskal algo.
// Checks each connection and adds it to the mst if  it connects two different groups, and then will merge them
TArray<TPair<FInt32Vector2, FInt32Vector2>> UDungeonGenUtils::CreateMST(const TArray<FInt32Vector2>& a_RoomCenters, const TArray<TPair<TPair<FInt32Vector2, FInt32Vector2>, float>>& a_Connections)
{
	// Minimum Spanning Tree (MST) --> Kruskal Algorithm
	TArray<TPair<FInt32Vector2, FInt32Vector2>> mst = {};
	TMap<FInt32Vector2, int32> roomGroups = {};
	int32 groupCounter = 0;

	for (int i = 0; i < a_RoomCenters.Num(); i++)
	{
		roomGroups.Add(a_RoomCenters[i], groupCounter++);
	}

	TArray<FInt32Vector2> keys;
	roomGroups.GenerateKeyArray(keys);

	for (int i = 0; i < a_Connections.Num(); i++)
	{
		FInt32Vector2 roomA = a_Connections[i].Key.Key; // todo check if this works!!
		FInt32Vector2 roomB = a_Connections[i].Key.Value;

		if (!roomGroups.Contains(roomA) || !roomGroups.Contains(roomB))
		{
			continue;
		}

		if (roomGroups[roomA] != roomGroups[roomB])
		{
			mst.Add(TPair<FInt32Vector2, FInt32Vector2>(roomA, roomB));
			int32 oldGroup = roomGroups[roomB];
			int32 newGroup = roomGroups[roomA];

			for (int j = 0; j < roomGroups.Num(); j++)
			{
				if (roomGroups[keys[j]] != oldGroup) continue;
				roomGroups[keys[j]] = newGroup;
			}
		}
	}

	return mst;
}

TArray<FInt32Vector2> UDungeonGenUtils::GetCellsToModifyFromVariant(const FDungeonRoom& a_Room)
{
	if (a_Room.m_RoomType == ERoomType::VARIANT_A)
	{
		//TArray<FInt32Vector2> cells;
		//int offsetX = 2, offsetY = 2;
		//int startX = a_Room.m_RoomOrigin.X, startY = a_Room.m_RoomOrigin.Y;
		//int endX = startX + a_Room.m_RoomCellLength - 1, endY = startY + a_Room.m_RoomCellWidth - 1;
		//
		//cells.Add(FInt32Vector2(startX + offsetX, startY + offsetY));
		//cells.Add(FInt32Vector2(endX - offsetX, startY + offsetY));
		//cells.Add(FInt32Vector2(startX + offsetX, endY - offsetY));
		//cells.Add(FInt32Vector2(endX - offsetX, endY - offsetY));

		TArray<FInt32Vector2> cells;
		int offset = 2;
		int startX = a_Room.m_RoomOrigin.X, startY = a_Room.m_RoomOrigin.Y;
		int length = a_Room.m_RoomCellLength, width = a_Room.m_RoomCellWidth;

		bool horizontal = FMath::RandBool();
		FInt32Vector2 center = a_Room.GetRoomCenter();

		if (horizontal)
		{
			int y1 = startY + offset;
			if (y1 == center.Y) y1 += 1;
			for (int x = startX; x < startX + length - offset; ++x)
				cells.Add(FInt32Vector2(x, y1));
			int y2 = startY + width - offset - 1;
			if (y2 == center.Y) y2 += 1;
			for (int x = startX + offset; x < startX + length; ++x)
				cells.Add(FInt32Vector2(x, y2));
		}
		else
		{
			int x1 = startX + offset;
			if (x1 == center.X) x1 += 1;
			for (int y = startY; y < startY + width - offset; ++y)
				cells.Add(FInt32Vector2(x1, y));
			int x2 = startX + length - offset - 1;
			if (x2 == center.X) x2 += 1;
			for (int y = startY + offset; y < startY + width; ++y)
				cells.Add(FInt32Vector2(x2, y));
		}
	
		return cells;
	}
	else if (a_Room.m_RoomType == ERoomType::VARIANT_B)
	{
		TArray<FInt32Vector2> cells;
		int offsetX = 2, offsetY = 2;
		int startX = a_Room.m_RoomOrigin.X, startY = a_Room.m_RoomOrigin.Y;
		int endX = startX + a_Room.m_RoomCellLength - 1, endY = startY + a_Room.m_RoomCellWidth - 1;

		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				cells.Add(FInt32Vector2(startX + offsetX + x, startY + offsetY + y));
				cells.Add(FInt32Vector2(endX - offsetX + x - 1, startY + offsetY + y));
				cells.Add(FInt32Vector2(startX + offsetX + x, endY - offsetY + y - 1));
				cells.Add(FInt32Vector2(endX - offsetX + x - 1, endY - offsetY + y - 1));
			}
		}

		return cells;
	}
	else if (a_Room.m_RoomType == ERoomType::VARIANT_C)
	{
		TArray<FInt32Vector2> cells;
		int offset = 2;
		int startX = a_Room.m_RoomOrigin.X, startY = a_Room.m_RoomOrigin.Y;
		int length = a_Room.m_RoomCellLength, width = a_Room.m_RoomCellWidth;

		if (length > width)
		{
			int midY = startY + width / 2;
			for (int x = startX + offset; x < startX + length - offset; x++)
			{
				cells.Add(FInt32Vector2(x, midY));
			}
		}
		else
		{
			int midX = startX + length / 2;
			for (int y = startY + offset; y < startY + length - offset; y++)
			{
				cells.Add(FInt32Vector2(midX, y));
			}
		}

		return cells;
	}
	else return TArray<FInt32Vector2>();
}
