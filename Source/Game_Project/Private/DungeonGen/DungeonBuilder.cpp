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
			//AStaticMeshActor* meshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator);
			if (m_Data->m_DungeonGrid[x][y] == ECellType::EMPTY)
			{

			}
			else if (m_Data->m_DungeonGrid[x][y] == ECellType::FLOOR)
			{
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
	for (int x = 0; x < m_Data->m_DungeonGrid.Num(); x++)
	{
		for (int y = 0; y < m_Data->m_DungeonGrid[x].Num(); y++)
		{
			if (m_Data->m_DungeonGrid[x][y] != ECellType::FLOOR) continue;

			int32 randomIndex = FMath::RandRange(0, m_DungeonTheme->m_WallMeshes.Num() - 1);

			TryPlaceWall(x + 1, y, { static_cast<float>((x + 1) * m_UnitSize) - m_WallOffset, static_cast<float>(y * m_UnitSize), 0.0f }, { 0, -90, 0 }, randomIndex);
			TryPlaceWall(x - 1, y, { static_cast<float>((x - 1) * m_UnitSize) + m_WallOffset, static_cast<float>(y * m_UnitSize), 0.0f }, { 0, 90, 0 }, randomIndex);
			TryPlaceWall(x, y + 1, { static_cast<float>(x * m_UnitSize), static_cast<float>((y + 1) * m_UnitSize) - m_WallOffset, 0.0f }, { 0, 180, 0 }, randomIndex);
			TryPlaceWall(x, y - 1, { static_cast<float>(x * m_UnitSize), static_cast<float>((y - 1) * m_UnitSize) + m_WallOffset, 0.0f }, { 0, 0, 0 }, randomIndex);

			//corner wall

			if (m_Data->m_DungeonGrid[x + 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x + 1][y - 1] = ECellType::WALLCORNER;
				FVector pos = { ((x + 1) * m_UnitSize) - m_WallOffset, ((y - 1) * m_UnitSize) + m_WallOffset, 0 };
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 0, 0 }); // testing!
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
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 90, 0 }); // testing!
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
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 180, 0 }); // testing!
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
				AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, -90, 0 }); // testing!
				if (meshActor)
				{
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
		}
	}
}

void UDungeonBuilder::TryPlaceWall(int32 a_GridX, int32 a_GridY, FVector a_Position, FRotator a_Rotation, int32 a_WallIndex)
{
	if (!IsWithinBounds(a_GridX, a_GridY)) return;
	if (m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::FLOOR) return;
	m_Data->m_DungeonGrid[a_GridX][a_GridY] = ECellType::WALL;
	AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), a_Position, a_Rotation); // testing!
	if (meshActor)
	{
		meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallMeshes[a_WallIndex].m_Mesh);
		meshActor->SetMobility(EComponentMobility::Static);
	}
}

void UDungeonBuilder::TryPlaceCornerWall(int32 a_GridX, int32 a_GridY, FVector a_Position, FRotator a_Rotation)
{
	if (!IsWithinBounds(a_GridX, a_GridY)) return;
	if (m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::FLOOR) return;
	m_Data->m_DungeonGrid[a_GridX][a_GridY] = ECellType::WALLCORNER;
	AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), a_Position, a_Rotation); // testing!
	if (meshActor)
	{
		meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
		meshActor->SetMobility(EComponentMobility::Static);
	}
}

bool UDungeonBuilder::IsWithinBounds(int32 a_GridX, int32 a_GridY)
{
	return a_GridX >= 0 && a_GridY >= 0 && a_GridX < m_Data->m_DungeonGrid.Num() && a_GridY < m_Data->m_DungeonGrid[0].Num();
}
