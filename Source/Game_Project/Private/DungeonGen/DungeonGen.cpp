// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonGen.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADungeonGen::ADungeonGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_DevCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DevCube"));
	RootComponent = m_DevCube;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(*DEFAULT_CUBE_PATH);
	
	if (cubeMesh.Succeeded()) m_DevCube->SetStaticMesh(cubeMesh.Object);
}

// Called when the game starts or when spawned
void ADungeonGen::BeginPlay()
{
	Super::BeginPlay();
	GenerateDungeon();
}

// Called every frame
void ADungeonGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGen::GenerateRooms()
{
	for (int i = 0; i < m_MaxRoomAmount; i++)
	{
		FInt32Vector2 newRoomSize = UDungeonGenUtils::GetRandomRoomSize(m_MinRoomSize, m_MaxRoomSize, m_DungeonRng);
		FInt32Vector2 newRoomOrigin = UDungeonGenUtils::GetRandomRoomOrigin(newRoomSize.X, newRoomSize.Y, m_GridLength, m_GridWidth, m_DungeonRng);
		FDungeonRoom newRoom = { newRoomOrigin, newRoomSize.X, newRoomSize.Y };
		bool overlapsWithRoom = false;

		for (int j = 0; j < m_Data.m_AllRooms.Num(); j++)
		{
			overlapsWithRoom = m_Data.m_AllRooms[j].OverlapsWith(newRoom);
			if (overlapsWithRoom) break;
		}

		if (overlapsWithRoom && !m_AllowOverlapingRooms) continue;

		newRoom.SetRoomType(ERoomType::EMPTY);
		m_Data.m_AllRooms.Add(newRoom);
		m_Data.m_AllRoomCenters.Add(newRoom.GetRoomCenter());
		TArray<FInt32Vector2> affectedCells = UDungeonGridUtils::CollectAffectedCells(newRoomOrigin, newRoomSize);
		UDungeonGridUtils::ChangeCellsInGrid(affectedCells, ECellType::FLOOR, m_Data.m_DungeonGrid);
	}
}

void ADungeonGen::GenerateCorridors()
{
	TArray<TPair<TPair<FInt32Vector2, FInt32Vector2>, float>> connections = {};
	for (int i = 0; i < m_Data.m_AllRoomCenters.Num(); i++)
	{
		for (int j = i + 1; j < m_Data.m_AllRoomCenters.Num(); j++)
		{
			float distance = FMath::Sqrt((float)FMath::Square(m_Data.m_AllRoomCenters[i].X - m_Data.m_AllRoomCenters[j].X) +
				(float)FMath::Square(m_Data.m_AllRoomCenters[i].Y - m_Data.m_AllRoomCenters[j].Y));
			TPair<FInt32Vector2, FInt32Vector2> connectionPoints = { m_Data.m_AllRoomCenters[i], m_Data.m_AllRoomCenters[j] };
			connections.Add({ connectionPoints, distance });
		}
	}

	connections.Sort([](const TPair<TPair<FInt32Vector2, FInt32Vector2>, float>& A, const TPair<TPair<FInt32Vector2, FInt32Vector2>, float>& B)
		{ return A.Value < B.Value; });

	m_Data.m_DungeonMST = UDungeonGenUtils::CreateMST(m_Data.m_AllRoomCenters, connections);

	TArray<FInt32Vector2> affectedCells = {};
	
	// Anzahl der Räume und erwartete Kanten
	UE_LOG(LogTemp, Log, TEXT("AllRoomCenters: %d, MST edges: %d"),
		m_Data.m_AllRoomCenters.Num(), m_Data.m_DungeonMST.Num());

	// alle Room-Center ausgeben
	for (int i = 0; i < m_Data.m_AllRoomCenters.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("RoomCenter: (%d,%d)"), m_Data.m_AllRoomCenters[i].X, m_Data.m_AllRoomCenters[i].Y);
	}

	for (int i = 0; i < m_Data.m_DungeonMST.Num(); i++)
	{
		for (int x = FMath::Min(m_Data.m_DungeonMST[i].Key.X, m_Data.m_DungeonMST[i].Value.X);
			x <= FMath::Max(m_Data.m_DungeonMST[i].Key.X, m_Data.m_DungeonMST[i].Value.X); x++)
		{
			affectedCells.Add({ x, m_Data.m_DungeonMST[i].Key.Y });
		}
		for (int y = FMath::Min(m_Data.m_DungeonMST[i].Key.Y, m_Data.m_DungeonMST[i].Value.Y);
			y <= FMath::Max(m_Data.m_DungeonMST[i].Key.Y, m_Data.m_DungeonMST[i].Value.Y); y++)
		{
			affectedCells.Add({ m_Data.m_DungeonMST[i].Value.X, y });
		}
	}
	UDungeonGridUtils::ChangeCellsInGrid(affectedCells, ECellType::FLOOR, m_Data.m_DungeonGrid);
}

void ADungeonGen::GenerateDungeon()
{
	m_DungeonRng.Initialize(m_Seed);
	m_Data = { m_MaxRoomAmount, m_GridLength, m_GridWidth };
	GenerateRooms();
	GenerateCorridors();

	// todo : makes unreal crash every time
	// m_Data.m_DungeonAdjacencyList = UDungeonGenUtils::BuildAdjacencyList(m_Data.m_AllRoomCenters, m_Data.m_DungeonMST);
	// m_Data.GetStartAndEndRoom(UDungeonGenUtils::DetermineDungeonDiameter(m_Data.m_AllRoomCenters, m_Data.m_DungeonAdjacencyList));
	// m_Data.GetDeadEndRooms();
	m_Builder = NewObject<UDungeonBuilder>(this);
	m_Builder->Init(m_UnitSize, m_DungeonTheme, &m_Data, GetWorld(), m_WallOffset);
	m_Builder->BuildFloor();
	m_Builder->BuildWall();
}

