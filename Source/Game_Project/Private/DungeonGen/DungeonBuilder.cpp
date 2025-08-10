// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonBuilder.h"
#include "Engine/StaticMeshActor.h"
#include "DungeonMesh.h"
#include "DungeonGenUtils.h"
#include "CustomChunkSystem/CustomChunkManager.h"
#include <Kismet/GameplayStatics.h>

void UDungeonBuilder::Init(float a_UnitSize, UDungeonTheme* a_Theme, FDungeonData* a_Data, UWorld* a_World, float a_WallOffset, ULevel* a_Level)
{
	m_UnitSize = a_UnitSize;
	m_DungeonTheme = a_Theme;
	m_Data = a_Data;
	m_WorldContext = a_World;
	m_WallOffset = a_WallOffset;
	m_Level = a_Level;
}

void UDungeonBuilder::BuildFloor()
{
	for (int x = 0; x < m_Data->m_DungeonGrid.Num(); x++)
	{
		for (int y = 0; y < m_Data->m_DungeonGrid[x].Num(); y++)
		{
			FVector pos = { static_cast<float>(x * m_UnitSize), static_cast<float>(y * m_UnitSize), 0.0f};
			FInt32Vector posOffset = m_DungeonTheme->m_FloorPosOffset;
			TArray<TArray<ECellType>> grid = m_Data->m_DungeonGrid;
			if (m_Data->m_DungeonGrid[x][y] == ECellType::FLOOR)
			{
				pos.X += (float)posOffset.X;
				pos.Y += (float)posOffset.Y;
				pos.Z += (float)posOffset.Z;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
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

			//O corner wall
			if (IsWithinBounds(x - 2, y))
			{
				if (m_Data->m_DungeonGrid[x - 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 2][y] == ECellType::FLOOR &&
					m_Data->m_DungeonGrid[x - 1][y - 1] == ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y + 1] == ECellType::FLOOR)
				{
					m_Data->m_DungeonGrid[x - 1][y] = ECellType::WALLO;
					FInt32Vector posOffset = m_DungeonTheme->m_WallOPosOffset;
					FVector pos = { ((x - 1) * m_UnitSize) - posOffset.X, (y * m_UnitSize) + posOffset.Y, 0 };
					FActorSpawnParameters params;
					params.OverrideLevel = m_Level;
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params));
					if (meshActor)
					{
						meshActor->SetMobility(EComponentMobility::Movable);
						meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOCornerMesh);
						meshActor->SetMobility(EComponentMobility::Static);
					}
				}
			}

			// U Corner Wall

			if (IsWithinBounds(x, y - 2))
			{
				if (m_Data->m_DungeonGrid[x][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 2] != ECellType::FLOOR &&
					m_Data->m_DungeonGrid[x - 1][y - 1] == ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y - 1] == ECellType::FLOOR)
				{
					m_Data->m_DungeonGrid[x][y - 1] = ECellType::WALLU;
					FInt32Vector posOffset = m_DungeonTheme->m_WallOPosOffset;
					FVector pos = { (x * m_UnitSize) + posOffset.X, ((y - 1) * m_UnitSize) - posOffset.Y, 0 };
					FActorSpawnParameters params;
					params.OverrideLevel = m_Level;
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0, 180, 0 }, params));
					if (meshActor)
					{
						meshActor->SetMobility(EComponentMobility::Movable);
						meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallUCornerMesh);
						meshActor->SetMobility(EComponentMobility::Static);
					}
				}
			}
			
			if (IsWithinBounds(x, y + 2))
			{
				if (m_Data->m_DungeonGrid[x][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 2] != ECellType::FLOOR &&
					m_Data->m_DungeonGrid[x - 1][y + 1] == ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y + 1] == ECellType::FLOOR)
				{
					m_Data->m_DungeonGrid[x][y + 1] = ECellType::WALLU;
					FInt32Vector posOffset = m_DungeonTheme->m_WallOPosOffset;
					FVector pos = { (x * m_UnitSize) - posOffset.X, ((y + 1) * m_UnitSize) + posOffset.Y, 0 };
					FActorSpawnParameters params;
					params.OverrideLevel = m_Level;
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params));
					if (meshActor)
					{
						meshActor->SetMobility(EComponentMobility::Movable);
						meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallUCornerMesh);
						meshActor->SetMobility(EComponentMobility::Static);
					}
				}
			}
			
			if (IsWithinBounds(x + 2, y))
			{
				if (m_Data->m_DungeonGrid[x + 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 2][y] != ECellType::FLOOR &&
					m_Data->m_DungeonGrid[x + 1][y - 1] == ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y + 1] == ECellType::FLOOR)
				{
					m_Data->m_DungeonGrid[x + 1][y] = ECellType::WALLU;
					FInt32Vector posOffset = m_DungeonTheme->m_WallOPosOffset;
					FVector pos = { ((x + 1) * m_UnitSize) + posOffset.X, (y * m_UnitSize) + posOffset.Y, 0 };
					FActorSpawnParameters params;
					params.OverrideLevel = m_Level;
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0, -90, 0 }, params));
					if (meshActor)
					{
						meshActor->SetMobility(EComponentMobility::Movable);
						meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallUCornerMesh);
						meshActor->SetMobility(EComponentMobility::Static);
					}
				}
			}
			
			if (IsWithinBounds(x - 2, y))
			{
				if (m_Data->m_DungeonGrid[x - 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 2][y] != ECellType::FLOOR &&
					m_Data->m_DungeonGrid[x - 1][y - 1] == ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y + 1] == ECellType::FLOOR)
				{
					m_Data->m_DungeonGrid[x - 1][y] = ECellType::WALLU;
					FInt32Vector posOffset = m_DungeonTheme->m_WallOPosOffset;
					FVector pos = { ((x - 1) * m_UnitSize) - posOffset.X, (y * m_UnitSize) - posOffset.Y, 0 };
					FActorSpawnParameters params;
					params.OverrideLevel = m_Level;
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0, 90, 0 }, params));
					if (meshActor)
					{
						meshActor->SetMobility(EComponentMobility::Movable);
						meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallUCornerMesh);
						meshActor->SetMobility(EComponentMobility::Static);
					}
				}
			}
			
		} 
	}
	
	for (int x = 0; x < m_Data->m_DungeonGrid.Num(); x++)
	{
		for (int y = 0; y < m_Data->m_DungeonGrid[x].Num(); y++)
		{
			if (m_Data->m_DungeonGrid[x][y] != ECellType::FLOOR) continue;

			//outer corner wall
			if (m_Data->m_DungeonGrid[x - 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y - 1] != ECellType::WALLO && 
				m_Data->m_DungeonGrid[x - 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] == ECellType::FLOOR && 
				m_Data->m_DungeonGrid[x - 1][y - 1] != ECellType::WALLU)
			{
				m_Data->m_DungeonGrid[x - 1][y - 1] = ECellType::WALLCONVEX;
				FInt32Vector posOffset = m_DungeonTheme->m_WallCornerOPosOffset;
				FVector pos = { ((x - 1) * m_UnitSize) + posOffset.X, ((y - 1) * m_UnitSize) + posOffset.Y, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerORotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 0 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
	
			if (m_Data->m_DungeonGrid[x - 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y + 1] != ECellType::WALLO && 
				m_Data->m_DungeonGrid[x - 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] == ECellType::FLOOR &&
				m_Data->m_DungeonGrid[x - 1][y + 1] != ECellType::WALLU)
			{
				m_Data->m_DungeonGrid[x - 1][y + 1] = ECellType::WALLCONVEX;
				FInt32Vector posOffset = m_DungeonTheme->m_WallCornerOPosOffset;
				FVector pos = { ((x - 1) * m_UnitSize) + posOffset.X, ((y + 1) * m_UnitSize) - posOffset.Y, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerORotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, -90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
	
			if (m_Data->m_DungeonGrid[x + 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y + 1] != ECellType::WALLO && 
				m_Data->m_DungeonGrid[x + 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] == ECellType::FLOOR &&
				m_Data->m_DungeonGrid[x + 1][y + 1] != ECellType::WALLU)
			{
				m_Data->m_DungeonGrid[x + 1][y + 1] = ECellType::WALLCONVEX;
				FInt32Vector posOffset = m_DungeonTheme->m_WallCornerOPosOffset;
				FVector pos = { ((x + 1) * m_UnitSize) - posOffset.X, ((y + 1) * m_UnitSize) - posOffset.Y, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerORotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 180 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
	
			if (m_Data->m_DungeonGrid[x + 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y - 1] != ECellType::WALLO && 
				m_Data->m_DungeonGrid[x + 1][y] == ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] == ECellType::FLOOR &&
				m_Data->m_DungeonGrid[x + 1][y - 1] != ECellType::WALLU)
			{
				m_Data->m_DungeonGrid[x + 1][y - 1] = ECellType::WALLCONVEX;
				FInt32Vector posOffset = m_DungeonTheme->m_WallCornerOPosOffset;
				FVector pos = { ((x + 1) * m_UnitSize) - posOffset.X, ((y - 1) * m_UnitSize) + posOffset.Y, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerORotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallOuterCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
		}
	}
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
				m_Data->m_DungeonGrid[x + 1][y - 1] = ECellType::WALLCONCAVE;
				FVector pos = { ((x + 1) * m_UnitSize) - m_WallOffset, ((y - 1) * m_UnitSize) + m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 0 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
			if (m_Data->m_DungeonGrid[x + 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x + 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x + 1][y + 1] = ECellType::WALLCONCAVE;
				FVector pos = { ((x + 1) * m_UnitSize) - m_WallOffset, ((y + 1) * m_UnitSize) - m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
			if (m_Data->m_DungeonGrid[x - 1][y + 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y + 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x - 1][y + 1] = ECellType::WALLCONCAVE;
				FVector pos = { ((x - 1) * m_UnitSize) + m_WallOffset, ((y + 1) * m_UnitSize) - m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 180 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
			if (m_Data->m_DungeonGrid[x - 1][y - 1] != ECellType::FLOOR && m_Data->m_DungeonGrid[x - 1][y] != ECellType::FLOOR && m_Data->m_DungeonGrid[x][y - 1] != ECellType::FLOOR)
			{
				m_Data->m_DungeonGrid[x - 1][y - 1] = ECellType::WALLCONCAVE;
				FVector pos = { ((x - 1) * m_UnitSize) + m_WallOffset, ((y - 1) * m_UnitSize) + m_WallOffset, 0 };
				FInt32Vector rotOffset = m_DungeonTheme->m_WallCornerRotOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, -90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallCornerMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
		}
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
			FActorSpawnParameters params;
			params.OverrideLevel = m_Level;
			ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
			AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), position, FRotator::ZeroRotator, params));
			//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), position, { 0, 0, 0 }); // testing!
			if (meshActor)
			{
				meshActor->SetMobility(EComponentMobility::Movable);
				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_StartCubeMesh);
				meshActor->SetMobility(EComponentMobility::Static);
			}
		}
		else if (curType == ERoomType::EXIT)
		{
			FInt32Vector2 intPos = m_Data->m_AllRooms[i].GetRoomCenter();
			FVector position = { (float)intPos.X * m_UnitSize, (float)intPos.Y * m_UnitSize, 0 };
			FActorSpawnParameters params;
		  params.OverrideLevel = m_Level;
			ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
			AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), position, FRotator::ZeroRotator, params));
			//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), position, { 0, 0, 0 }); // testing!
			if (meshActor)
			{
				meshActor->SetMobility(EComponentMobility::Movable);
				meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_ExitCubeMesh);
				meshActor->SetMobility(EComponentMobility::Static);
			}
		}
	}
}

void UDungeonBuilder::BuildDecorationObjects()
{
	for (int i = 0; i < m_Data->m_AllRooms.Num(); i++)
	{
		ERoomType curType = m_Data->m_AllRooms[i].m_RoomType;

		if (curType == ERoomType::VARIANT_A || curType == ERoomType::VARIANT_B || curType == ERoomType::VARIANT_C)
		{
			for (int x = m_Data->m_AllRooms[i].m_RoomOrigin.X; x < m_Data->m_AllRooms[i].m_RoomOrigin.X + m_Data->m_AllRooms[i].m_RoomCellLength; x++)
			{
				for (int y = m_Data->m_AllRooms[i].m_RoomOrigin.Y; y < m_Data->m_AllRooms[i].m_RoomOrigin.Y + m_Data->m_AllRooms[i].m_RoomCellWidth; y++)
				{
					TryPlacePrefabCornerFacingCenter(m_Data->m_DungeonGrid, x, y, m_Data->m_AllRooms[i], m_DungeonTheme->m_DecorationVarB[FMath::RandRange(0, m_DungeonTheme->m_DecorationVarB.Num() - 1)].m_Mesh, 80);
				}
			}
		}
		else if (curType == ERoomType::EMPTY || curType == ERoomType::REWARD)
		{
			int32 randomIndex = FMath::RandRange(0, m_DungeonTheme->m_DecorationVarA.Num() - 1);
			for (int x = m_Data->m_AllRooms[i].m_RoomOrigin.X; x < m_Data->m_AllRooms[i].m_RoomOrigin.X + m_Data->m_AllRooms[i].m_RoomCellLength; x++)
			{
				for (int y = m_Data->m_AllRooms[i].m_RoomOrigin.Y; y < m_Data->m_AllRooms[i].m_RoomOrigin.Y + m_Data->m_AllRooms[i].m_RoomCellWidth; y++)
				{
					TryPlacePrefabCornerOrthoRotation(m_Data->m_DungeonGrid, x, y, m_Data->m_AllRooms[i], m_DungeonTheme->m_DecorationVarA[randomIndex].m_Mesh, 100);
				}
			}
		}
	}
}

void UDungeonBuilder::BuildTorches()
{
	for (int i = 0; i < m_Data->m_AllRooms.Num(); i++)
	{
		for (int x = m_Data->m_AllRooms[i].m_RoomOrigin.X; x <= m_Data->m_AllRooms[i].m_RoomOrigin.X + m_Data->m_AllRooms[i].m_RoomCellLength; x++)
		{
			for (int y = m_Data->m_AllRooms[i].m_RoomOrigin.Y; y <= m_Data->m_AllRooms[i].m_RoomOrigin.Y + m_Data->m_AllRooms[i].m_RoomCellWidth; y++)
			{
				if (m_Data->m_DungeonGrid[x + 1][y + 1] == ECellType::WALLCONCAVE)
				{
					if (m_Data->m_DungeonGrid[x - 1][y + 1] == ECellType::WALL)
					{
						PlaceTorchAt(x - 1, y, FVector(0.0f, 215.0f, 0.0f), FRotator(0, 180, 0));
					}
					if (m_Data->m_DungeonGrid[x + 1][y - 1] == ECellType::WALL)
					{
						PlaceTorchAt(x, y - 1, FVector(215.0f, 0.0f, 0.0f), FRotator(0, 90, 0));
					}
				}
				else if (m_Data->m_DungeonGrid[x - 1][y + 1] == ECellType::WALLCONCAVE)
				{
					if (m_Data->m_DungeonGrid[x - 1][y - 1] == ECellType::WALL)
					{
						PlaceTorchAt(x, y - 1, FVector(-215.0f, 0.0f, 0.0f), FRotator(0, 270, 0));
					}
					if (m_Data->m_DungeonGrid[x + 1][y + 1] == ECellType::WALL)
					{
						PlaceTorchAt(x + 1, y, FVector(0.0f, 215.0f, 0.0f), FRotator(0, 180, 0));
					}
				}
				else if (m_Data->m_DungeonGrid[x + 1][y - 1] == ECellType::WALLCONCAVE)
				{
					if (m_Data->m_DungeonGrid[x - 1][y - 1] == ECellType::WALL)
					{
						PlaceTorchAt(x - 1, y, FVector(0.0f, -215.0f, 0.0f), FRotator(0, 0, 0));
					}
					if (m_Data->m_DungeonGrid[x + 1][y + 1] == ECellType::WALL)
					{
						PlaceTorchAt(x, y + 1, FVector(215.0f, 0.0f, 0.0f), FRotator(0, 90, 0));
					}
				}
				else if (m_Data->m_DungeonGrid[x - 1][y - 1] == ECellType::WALLCONCAVE)
				{
					if (m_Data->m_DungeonGrid[x - 1][y + 1] == ECellType::WALL)
					{
						PlaceTorchAt(x, y + 1, FVector(-215.0f, 0.0f, 0.0f), FRotator(0, -90, 0));
					}
					if (m_Data->m_DungeonGrid[x + 1][y - 1] == ECellType::WALL)
					{
						PlaceTorchAt(x + 1, y, FVector(0.0f, -215.0f, 0.0f), FRotator(0, 0, 0));
					}
				}
			}
		}
	}
}

void UDungeonBuilder::BuildBossRoom()
{
	for (int i = 0; i < m_Data->m_AllRooms.Num(); i++)
	{
		ERoomType curType = m_Data->m_AllRooms[i].m_RoomType;
		FInt32Vector2 roomOrigin = m_Data->m_AllRooms[i].m_RoomOrigin;
		int32 roomCellLength = m_Data->m_AllRooms[i].m_RoomCellLength;
		int32 roomCellWidth = m_Data->m_AllRooms[i].m_RoomCellWidth;

		if (curType != ERoomType::EXIT) continue;

		for (int x = roomOrigin.X - 1; x < roomOrigin.X + roomCellLength + 1; x++)
		{
			for (int y = roomOrigin.Y - 1; y < roomOrigin.Y + roomCellWidth + 1; y++)
			{
				if (x == roomOrigin.X - 1 || x == roomOrigin.X + roomCellLength|| y == roomOrigin.Y - 1 || y == roomOrigin.Y + roomCellWidth)
				{
					TryPlaceDoor(x, y, m_Data->m_AllRooms[i]);
					continue;
				}

				FVector pos = { static_cast<float>(x * m_UnitSize), static_cast<float>(y * m_UnitSize), 0.0f };
				FInt32Vector posOffset = m_DungeonTheme->m_FloorPosOffset;
				TArray<TArray<ECellType>> grid = m_Data->m_DungeonGrid;
				pos.X += (float)posOffset.X;
				pos.Y += (float)posOffset.Y;
				pos.Z += (float)posOffset.Z + m_DungeonTheme->m_BossFloorZOffset;
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, {0, 90, 0}, params));
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_BossFloorMesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}
			}
		}
	}
}

void UDungeonBuilder::GenerateEnemies()
{
	if (m_DungeonTheme->m_EnemyCharacters.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning: TArray EnemyCharacters is empty!"));
		return;
	}

	for (int i = 0; i < m_Data->m_AllRooms.Num(); i++)
	{
		ERoomType curType = m_Data->m_AllRooms[i].m_RoomType;
		if (curType == ERoomType::EXIT || curType == ERoomType::ENTRANCE || curType == ERoomType::BOSS) continue;

		FInt32Vector2 intPos = m_Data->m_AllRooms[i].GetRoomCenter();
		if (m_Data->m_DungeonGrid[intPos.X][intPos.Y] != ECellType::FLOOR)
		{
			bool shouldBreak = false;
			for (int x = m_Data->m_AllRooms[i].m_RoomOrigin.X; x < m_Data->m_AllRooms[i].m_RoomOrigin.X + m_Data->m_AllRooms[i].m_RoomCellLength; x++)
			{
				if (shouldBreak) break;
				for (int y = m_Data->m_AllRooms[i].m_RoomOrigin.Y; y < m_Data->m_AllRooms[i].m_RoomOrigin.Y + m_Data->m_AllRooms[i].m_RoomCellWidth; y++)
				{
					if (m_Data->m_DungeonGrid[x][y] == ECellType::FLOOR)
					{
						intPos.X = x;
						intPos.Y = y;
						shouldBreak = true;
						break;
					}
				}
			}
		}
		FVector position = { (float)intPos.X * m_UnitSize, (float)intPos.Y * m_UnitSize, 0 };

		int enemiesInRoom = FMath::RandRange(2, 5);
		for (int j = 0; j < enemiesInRoom; j++)
		{
			int32 randIndex = FMath::RandRange(0, m_DungeonTheme->m_EnemyCharacters.Num() - 1);
			TSubclassOf<AEnemyCharacter> enemyToSpawn = m_DungeonTheme->m_EnemyCharacters[randIndex];

			if (enemyToSpawn)
			{
				//GetWorld()->SpawnActor<AEnemyCharacter>(enemyToSpawn, position, {0, 0, 0});
				FActorSpawnParameters params;
				params.OverrideLevel = m_Level;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				chunkManager->SpawnActorInChunk(enemyToSpawn, position, FRotator::ZeroRotator, params);
			}
		}
	}
}

void UDungeonBuilder::SpawnBossEnemyRandom()
{
	if (m_DungeonTheme->m_BossCharacters.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning: TArray BossCharacters is empty!"));
		return;
	}

	int randomBossIndex = FMath::RandRange(0, m_DungeonTheme->m_BossCharacters.Num() - 1);

	for (int i = 0; i < m_Data->m_AllRooms.Num(); i++)
	{
		ERoomType curType = m_Data->m_AllRooms[i].m_RoomType;
		if (curType != ERoomType::EXIT && curType != ERoomType::BOSS) continue;

		FInt32Vector2 intPos = m_Data->m_AllRooms[i].GetRoomCenter();
		FVector position = { (float)intPos.X * m_UnitSize, (float)intPos.Y * m_UnitSize, 0.0f };

		TSubclassOf<AEnemyCharacter> bossToSpawn = m_DungeonTheme->m_BossCharacters[randomBossIndex];

		if (bossToSpawn)
		{
			FActorSpawnParameters params;
			params.OverrideLevel = m_Level;
			ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
			chunkManager->SpawnActorInChunk(bossToSpawn, position, FRotator::ZeroRotator, params);
		}
	}
}

void UDungeonBuilder::TryPlaceWall(int32 a_GridX, int32 a_GridY, const FVector& a_Position, const FRotator& a_Rotation, int32 a_WallIndex) const
{
	if (!IsWithinBounds(a_GridX, a_GridY)) return;
	if (m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::FLOOR || m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::WALLCONVEX || 
		m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::WALLO || m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::WALLU
		/*m_Data->m_DungeonGrid[a_GridX][a_GridY] == ECellType::WALLCONCAVE*/) return;
	m_Data->m_DungeonGrid[a_GridX][a_GridY] = ECellType::WALL;
	FInt32Vector rotOffset = m_DungeonTheme->m_WallRotOffset;
	FRotator rotation = { a_Rotation.Pitch + (float)rotOffset.X, a_Rotation.Yaw + (float)rotOffset.Y, a_Rotation.Roll + (float)rotOffset.Z };
	FActorSpawnParameters params;
	params.OverrideLevel = m_Level;
	ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
	AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), a_Position, rotation, params));
	//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), a_Position, rotation); // testing!
	if (meshActor)
	{
		meshActor->SetMobility(EComponentMobility::Movable);
		meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_WallMeshes[a_WallIndex].m_Mesh);
		meshActor->SetMobility(EComponentMobility::Static);
	}
}

void UDungeonBuilder::TryPlaceDoor(int32 a_GridX, int32 a_GridY, const FDungeonRoom& a_Room) const
{
	if (!IsWithinBounds(a_GridX, a_GridY)) return;
	UE_LOG(LogTemp, Error, TEXT("Door tries to spawn"));
	TArray<TArray<ECellType>> grid = m_Data->m_DungeonGrid;

	if (grid[a_GridX][a_GridY] != ECellType::FLOOR && grid[a_GridX][a_GridY] != ECellType::FLOORCORRIDOR) return;
	UE_LOG(LogTemp, Error, TEXT("Door has been spawned"));
	bool canBePlaced = false;
	FVector pos = { a_GridX * m_UnitSize, (float)a_GridY * m_UnitSize, -400.0f };
	FInt32Vector2 roomCenter = a_Room.GetRoomCenter();
	int32 rotationY = UDungeonGenUtils::GetOrthogonalRotationBasedOnCenter(pos, FVector(roomCenter.X, roomCenter.Y, 0) * m_UnitSize);
	FRotator rot = { 0, (float)rotationY, 0 };
	FActorSpawnParameters params;
	params.OverrideLevel = m_Level;

	ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
	chunkManager->SpawnActorInChunk(m_DungeonTheme->m_DoorMesh, pos, rot, params);
}

bool UDungeonBuilder::IsWithinBounds(int32 a_GridX, int32 a_GridY) const
{
	return a_GridX >= 0 && a_GridY >= 0 && a_GridX < m_Data->m_DungeonGrid.Num() && a_GridY < m_Data->m_DungeonGrid[0].Num();
}

bool UDungeonBuilder::TryPlacePrefabCornerFacingCenter(TArray<TArray<ECellType>> a_Grid, int32 a_GridX, int32 a_GridY, FDungeonRoom a_Room, UStaticMesh *a_Mesh, int32 a_Probabilty)
{
	if (a_Grid[a_GridX][ a_GridY] == ECellType::FLOOR && UDungeonGenUtils::CellIsNeighbourOfPosition(a_GridX, a_GridY, ECellType::WALLCONCAVE, a_Grid, true))
	{
		if (FMath::RandRange(0, 100) < 100 - a_Probabilty) return false;
		FVector targetPosition = FVector(a_GridX * m_UnitSize, a_GridY * m_UnitSize, 0);
		FInt32Vector2 roomCenter = a_Room.GetRoomCenter();
		FVector direction = FVector(roomCenter.X, roomCenter.Y, 0) * m_UnitSize - targetPosition;
		direction.Z = 0;
		FRotator targetRotation = FRotationMatrix::MakeFromX(direction).Rotator();
		FActorSpawnParameters params;
		params.OverrideLevel = m_Level;
		ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
		AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), targetPosition, targetRotation, params));
		//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), targetPosition, targetRotation); // testing!
		if (meshActor)
		{
			meshActor->SetMobility(EComponentMobility::Movable);
			meshActor->GetStaticMeshComponent()->SetStaticMesh(a_Mesh);
			meshActor->SetMobility(EComponentMobility::Static);
		}
	}
	return false;
}

bool UDungeonBuilder::TryPlacePrefabCornerOrthoRotation(TArray<TArray<ECellType>> a_Grid, int32 a_GridX, int32 a_GridY, FDungeonRoom a_Room, UStaticMesh* a_Mesh, int32 a_Probabilty)
{
	if (a_Grid[a_GridX][a_GridY] == ECellType::FLOOR && UDungeonGenUtils::CellIsNeighbourOfPosition(a_GridX, a_GridY, ECellType::WALLCONCAVE, a_Grid, true))
	{
		if (FMath::RandRange(0, 100) < 100 - a_Probabilty) return false;
		FVector targetPosition = FVector(a_GridX * m_UnitSize, a_GridY * m_UnitSize, 0);
		FInt32Vector2 roomCenter = a_Room.GetRoomCenter();
		int32 rotationY = UDungeonGenUtils::GetOrthogonalRotationBasedOnCenter(targetPosition, FVector(roomCenter.X, roomCenter.Y, 0) * m_UnitSize);
		FRotator targetRotation = FRotator(0, (float)rotationY, 0);
		FActorSpawnParameters params;
		params.OverrideLevel = m_Level;
		ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
		AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), targetPosition, targetRotation, params));
		//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), targetPosition, targetRotation); // testing!
		if (meshActor)
		{
			meshActor->SetMobility(EComponentMobility::Movable);
			meshActor->GetStaticMeshComponent()->SetStaticMesh(a_Mesh);
			meshActor->SetMobility(EComponentMobility::Static);
		}
	}
	return false;
}

void UDungeonBuilder::PlaceTorchAt(int32 a_X, int32 a_Y, const FVector& a_PositionOffset, const FRotator& a_RotationOffset)
{
	if (m_Data->m_DungeonGrid[a_X][a_Y] != ECellType::FLOOR) return;
	FVector pos = FVector(a_X * m_UnitSize, a_Y * m_UnitSize, 150.0f);
	pos += a_PositionOffset;

	FRotator rot = FRotator::ZeroRotator;
	rot += a_RotationOffset;

	FActorSpawnParameters params;
	params.OverrideLevel = m_Level;
	ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
	chunkManager->SpawnActorInChunk(m_DungeonTheme->m_TorchMesh, pos, rot, params);
}
