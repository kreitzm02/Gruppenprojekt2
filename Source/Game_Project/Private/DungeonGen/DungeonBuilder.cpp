// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonBuilder.h"
#include "Engine/StaticMeshActor.h"

void UDungeonBuilder::Init(float a_UnitSize, UDungeonTheme* a_Theme, FDungeonData* a_Data, UWorld* a_World, float a_WallOffset)
{
	m_UnitSize = a_UnitSize;
	m_DungeonTheme = a_Theme;
	m_Data = a_Data;
	m_WorldContext = a_World;
	m_WallOffset = a_WallOffset;
}

void UDungeonBuilder::BuildFloor()
{
	for (int x = 0; x < m_Data->m_DungeonGrid.Num(); x++)
	{
		for (int y = 0; y < m_Data->m_DungeonGrid[x].Num(); y++)
		{
			FVector pos = { static_cast<float>(x * m_UnitSize), static_cast<float>(y * m_UnitSize), 0.0f};
			FInt32Vector posOffset = m_DungeonTheme->m_FloorPosOffset;
			//AStaticMeshActor* meshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator);
			if (m_Data->m_DungeonGrid[x][y] == ECellType::EMPTY || m_Data->m_DungeonGrid[x][y] == ECellType::EMPTYFORCED)
			{
				pos.Z = 395.0f;
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
				if (meshActor)
				{
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_VoidMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
			else if (m_Data->m_DungeonGrid[x][y] == ECellType::FLOOR)
			{
				pos.X += (float)posOffset.X;
				pos.Y += (float)posOffset.Y;
				pos.Z += (float)posOffset.Z;
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
				if (meshActor)
				{
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_FloorMeshes[0].m_Mesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
		}
	}
}

void UDungeonBuilder::BuildWall()
{
	//TODO outer corner fertigstellen, damit das snake tileset passt.
	//for (int x = 0; x < m_Data->m_DungeonGrid.Num(); x++)
	//{
	//	for (int y = 0; y < m_Data->m_DungeonGrid[x].Num(); y++)
	//	{
	//		if (m_Data->m_DungeonGrid[x][y] != ECellType::FLOOR) continue;
	//
	//		//outer corner wall
	//		if (m_Data->m_DungeonGrid[x - 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] == ECellType::FLOOR)
	//		{
	//			m_Data->m_DungeonGrid[x - 1][y - 1] = ECellType::WALLCORNERO;
	//			FVector pos = { ((x - 1) * m_UnitSize) - m_WallOffset, ((y - 1) * m_UnitSize) + m_WallOffset, 0 };
	//			FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
	//			AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 0, 0 }); // testing!
	//			if (meshActor)
	//			{
	//				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
	//				meshActor->SetMobility(EComponentMobility::Static);
	//			}
	//		}
	//
	//		if (m_Data->m_DungeonGrid[x - 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] == ECellType::FLOOR)
	//		{
	//			m_Data->m_DungeonGrid[x - 1][y + 1] = ECellType::WALLCORNERO;
	//			FVector pos = { ((x - 1) * m_UnitSize) - m_WallOffset, ((y + 1) * m_UnitSize) + m_WallOffset, 0 };
	//			FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
	//			AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, -90, 0 }); // testing!
	//			if (meshActor)
	//			{
	//				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
	//				meshActor->SetMobility(EComponentMobility::Static);
	//			}
	//		}
	//
	//		if (m_Data->m_DungeonGrid[x + 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] == ECellType::FLOOR)
	//		{
	//			m_Data->m_DungeonGrid[x + 1][y + 1] = ECellType::WALLCORNERO;
	//			FVector pos = { ((x + 1) * m_UnitSize) - m_WallOffset, ((y + 1) * m_UnitSize) + m_WallOffset, 0 };
	//			FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
	//			AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 180, 0 }); // testing!
	//			if (meshActor)
	//			{
	//				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
	//				meshActor->SetMobility(EComponentMobility::Static);
	//			}
	//		}
	//
	//		if (m_Data->m_DungeonGrid[x + 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] == ECellType::FLOOR)
	//		{
	//			m_Data->m_DungeonGrid[x + 1][y - 1] = ECellType::WALLCORNERO;
	//			FVector pos = { ((x + 1) * m_UnitSize) - m_WallOffset, ((y - 1) * m_UnitSize) + m_WallOffset, 0 };
	//			FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
	//			AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 90, 0 }); // testing!
	//			if (meshActor)
	//			{
	//				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
	//				meshActor->SetMobility(EComponentMobility::Static);
	//			}
	//		}
	//	}
	//}
	for (int x = 0; x < m_Data->m_DungeonGrid.Num(); x++)
	{
		for (int y = 0; y < m_Data->m_DungeonGrid[x].Num(); y++)
		{
			if (m_Data->m_DungeonGrid[x][y] != ECellType::FLOOR) continue;

			int32 randomIndex = FMath::RandRange(0, m_DungeonTheme->m_WallMeshes.Num() - 1);
			
			TryPlaceWall(x + 1, y, { static_cast<float>((x + 1) * m_UnitSize) - m_WallOffset, static_cast<float>(y * m_UnitSize), 0.0f }, { 0, 90, 0 }, randomIndex);
			TryPlaceWall(x - 1, y, { static_cast<float>((x - 1) * m_UnitSize) + m_WallOffset, static_cast<float>(y * m_UnitSize), 0.0f }, { 0, -90, 0 }, randomIndex);
			TryPlaceWall(x, y + 1, { static_cast<float>(x * m_UnitSize), static_cast<float>((y + 1) * m_UnitSize) - m_WallOffset, 0.0f }, { 0, 180, 0 }, randomIndex);
			TryPlaceWall(x, y - 1, { static_cast<float>(x * m_UnitSize), static_cast<float>((y - 1) * m_UnitSize) + m_WallOffset, 0.0f }, { 0, 0, 0 }, randomIndex);
				
			//inner corner wall

			if (m_Data->m_DungeonGrid[x + 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x + 1][y - 1] = ECellType::WALLCORNER;
				FVector pos = { ((x + 1) * m_UnitSize) - m_WallOffset, ((y - 1) * m_UnitSize) + m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 0 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
				if (meshActor)
				{
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
			if (m_Data->m_DungeonGrid[x + 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x + 1][y + 1] = ECellType::WALLCORNER;
				FVector pos = { ((x + 1) * m_UnitSize) - m_WallOffset, ((y + 1) * m_UnitSize) - m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
				if (meshActor)
				{
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
			if (m_Data->m_DungeonGrid[x - 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x - 1][y + 1] = ECellType::WALLCORNER;
				FVector pos = { ((x - 1) * m_UnitSize) + m_WallOffset, ((y + 1) * m_UnitSize) - m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 180 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
				if (meshActor)
				{
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
			if (m_Data->m_DungeonGrid[x - 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x - 1][y - 1] = ECellType::WALLCORNER;
				FVector pos = { ((x - 1) * m_UnitSize) + m_WallOffset, ((y - 1) * m_UnitSize) + m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, -90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
				if (meshActor)
				{
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
		}
	}

	const int32 Width = m_Data->m_DungeonGrid.Num();
	const int32 Height = (Width > 0 ? m_Data->m_DungeonGrid[0].Num() : 0);

	for (int32 Y = 0; Y < Height; ++Y)
	{
		FString Line;
		Line.Reserve(Width);

		for (int32 X = 0; X < Width; ++X)
		{
			ECellType Cell = m_Data->m_DungeonGrid[X][Y];
			// Mappe den Cell-Typ auf ein einzelnes Zeichen
			TCHAR Ch;
			switch (Cell)
			{
			case ECellType::EMPTY:        Ch = TEXT('.'); break;
			case ECellType::EMPTYFORCED:  Ch = TEXT('F'); break;
			case ECellType::FLOOR:        Ch = TEXT(' '); break;
			case ECellType::WALL:         Ch = TEXT('#'); break;
			case ECellType::WALLCORNERO:  Ch = TEXT('O'); break;
			case ECellType::WALLCORNER:   Ch = TEXT('C'); break;
			default:                      Ch = TEXT('?'); break;
			}
			Line.AppendChar(Ch);
		}

		// Ausgabe in die UE4-Logkonsole
		UE_LOG(LogTemp, Log, TEXT("%s"), *Line);
	}
}

void UDungeonBuilder::BuildDebugObjects()
{
	for (int i = 0; i < m_Data->m_AllRooms.Num(); i++)
	{
		ERoomType curType = m_Data->m_AllRooms[i].m_RoomType;

		if (curType == ERoomType::ENTRANCE)
		{
			FInt32Vector2 intPos = m_Data->m_AllRooms[i].GetRoomCenter();
			FVector position = { (float)intPos.X * m_UnitSize, (float)intPos.Y * m_UnitSize, 0 };
			AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), position, { 0, 0, 0 }); // testing!
			if (meshActor)
			{
				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_StartCubeMesh);
				meshActor->SetMobility(EComponentMobility::Static);
			}
		}
		else if (curType == ERoomType::EXIT)
		{
			FInt32Vector2 intPos = m_Data->m_AllRooms[i].GetRoomCenter();
			FVector position = { (float)intPos.X * m_UnitSize, (float)intPos.Y * m_UnitSize, 0 };
			AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), position, { 0, 0, 0 }); // testing!
			if (meshActor)
			{
				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_ExitCubeMesh);
				meshActor->SetMobility(EComponentMobility::Static);
			}
		}
	}
}

void UDungeonBuilder::TryPlaceWall(int32 a_GridX, int32 a_GridY, const FVector& a_Position, const FRotator& a_Rotation, int32 a_WallIndex) const
{
	if (!IsWithinBounds(a_GridX, a_GridY)) return;
	if (m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::FLOOR || m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::WALLCORNERO) return;
	m_Data->m_DungeonGrid[a_GridX][a_GridY] = ECellType::WALL;
	FInt32Vector rotOffset = m_DungeonTheme->m_WallRotOffset;
	FRotator rotation = { a_Rotation.Pitch + (float)rotOffset.X, a_Rotation.Yaw + (float)rotOffset.Y, a_Rotation.Roll + (float)rotOffset.Z };
	AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), a_Position, rotation); // testing!
	if (meshActor)
	{
		meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallMeshes[a_WallIndex].m_Mesh);
		meshActor->SetMobility(EComponentMobility::Static);
	}
}

bool UDungeonBuilder::IsWithinBounds(int32 a_GridX, int32 a_GridY) const
{
	return a_GridX >= 0 && a_GridY >= 0 && a_GridX < m_Data->m_DungeonGrid.Num() && a_GridY < m_Data->m_DungeonGrid[0].Num();
}
