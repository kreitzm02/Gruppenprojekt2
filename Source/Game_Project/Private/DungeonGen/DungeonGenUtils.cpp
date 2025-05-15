// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonGenUtils.h"

FInt32Vector2 UDungeonGenUtils::GetRandomRoomSize(int32 a_Min, int32 a_Max)
{
	return FInt32Vector2(FMath::RandRange(a_Min, a_Max) ,FMath::RandRange(a_Min, a_Max));
}

FInt32Vector2 UDungeonGenUtils::GetRandomRoomOrigin(int32 a_RoomLengthX, int32 a_RoomWidthY, int32 a_GridLengthX, int32 a_GridWidthY)
{
	return FInt32Vector2(FMath::RandRange(1, a_GridLengthX - a_RoomLengthX - 1), FMath::RandRange(1, a_GridWidthY - a_RoomWidthY - 1));
}

// this will create a dictionary which will get each room their corresponding neighbour rooms ( room centers are used for this )
TMap<FInt32Vector2, TArray<FInt32Vector2>> UDungeonGenUtils::BuildAdjacencyList(const TArray<FInt32Vector2>& a_RoomCenters, const TArray<TPair<FInt32Vector2, FInt32Vector2>>& a_MST)
{
	TMap<FInt32Vector2, TArray<FInt32Vector2>> adjacencyList = {};
	for (int i = 0; i < a_RoomCenters.Num(); i++)
	{
		adjacencyList[a_RoomCenters[i]] = TArray<FInt32Vector2>();
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
		distances[a_RoomCenters[i]] = -1;
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
