// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonBuilder.h"
#include "Engine/StaticMeshActor.h"
#include "DungeonMesh.h"
#include "DungeonGenUtils.h"
#include "CustomChunkSystem/CustomChunkManager.h"
#include <Kismet/GameplayStatics.h>

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
			TArray<TArray<ECellType>> grid = m_Data->m_DungeonGrid;
			//AStaticMeshActor* meshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator);
			if (grid[x][y] == ECellType::EMPTY || grid[x][y] == ECellType::EMPTYFORCED)
			{
				// TODO : replaced with modified meshes that have a "black void" side. need to check if this method is reliable.
				//pos.Z = 395.0f;
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
				//if (meshActor)
				//{
				//	meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_VoidMesh);
				//	meshActor->SetMobility(EComponentMobility::Movable);
				//
				//	if (IsWithinBounds(x + 1, y) && IsWithinBounds(x - 1, y) && IsWithinBounds(x, y + 1) && IsWithinBounds(x, y - 1))
				//	{
				//		// 4 border
				//		if (grid[x + 1][y] == ECellType::FLOOR && grid[x][y + 1] == ECellType::FLOOR &&
				//			grid[x - 1][y] == ECellType::FLOOR && grid[x][y - 1] == ECellType::FLOOR)
				//		{
				//			meshActor->SetActorScale3D(FVector(0.5, 0.5, 1));
				//		}
				//
				//		// 3 border
				//		if (grid[x + 1][y] == ECellType::FLOOR && grid[x - 1][y] == ECellType::FLOOR &&
				//			grid[x][y + 1] == ECellType::FLOOR && grid[x][y - 1] != ECellType::FLOOR)
				//		{
				//			meshActor->SetActorScale3D(FVector(0.5, 0.75, 1));
				//			meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//		}
				//
				//		if (grid[x + 1][y] == ECellType::FLOOR && grid[x - 1][y] == ECellType::FLOOR &&
				//			grid[x][y - 1] == ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR)
				//		{
				//			meshActor->SetActorScale3D(FVector(0.5, 0.75, 1));
				//			meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, 50, 0));
				//		}
				//
				//		if (grid[x + 1][y] != ECellType::FLOOR && grid[x - 1][y] == ECellType::FLOOR &&
				//			grid[x][y + 1] == ECellType::FLOOR && grid[x][y - 1] == ECellType::FLOOR)
				//		{
				//			meshActor->SetActorScale3D(FVector(0.75, 0.5, 1));
				//			meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(50, 0, 0));
				//		}
				//
				//		if (grid[x + 1][y] == ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR &&
				//			grid[x][y - 1] == ECellType::FLOOR && grid[x][y + 1] == ECellType::FLOOR)
				//		{
				//			meshActor->SetActorScale3D(FVector(0.75, 0.5, 1));
				//			meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(-50, 0, 0));
				//		}
				//
				//		// 2 border
				//		if (grid[x + 1][y] == ECellType::FLOOR && grid[x - 1][y] == ECellType::FLOOR &&
				//			grid[x][y - 1] != ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR)
				//		{
				//			meshActor->SetActorScale3D(FVector(0.5, 1, 1));
				//		}
				//
				//		if (grid[x + 1][y] != ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR &&
				//			grid[x][y - 1] == ECellType::FLOOR && grid[x][y + 1] == ECellType::FLOOR)
				//		{
				//			meshActor->SetActorScale3D(FVector(1, 0.5, 1));
				//		}
				//
				//		// 1 border
				//		if (grid[x + 1][y] == ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR &&
				//			grid[x][y - 1] != ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR)
				//		{
				//			if (IsWithinBounds(x - 1, y - 1) && grid[x - 1][y - 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0, 0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else if (IsWithinBounds(x - 1, y + 1) && grid[x - 1][y + 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0, 0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else
				//			{
				//				meshActor->SetActorScale3D(FVector(0.75, 1, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(-50, 0, 0));
				//			}
				//		}
				//
				//		if (grid[x + 1][y] != ECellType::FLOOR && grid[x - 1][y] == ECellType::FLOOR &&
				//			grid[x][y - 1] != ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR)
				//		{
				//			if (IsWithinBounds(x + 1, y - 1) && grid[x + 1][y - 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0, 0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else if (IsWithinBounds(x + 1, y + 1) && grid[x + 1][y + 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0, 0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else
				//			{
				//				meshActor->SetActorScale3D(FVector(0.75, 1, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(50, 0, 0));
				//			}
				//		}
				//
				//		if (grid[x + 1][y] != ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR &&
				//			grid[x][y - 1] == ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR)
				//		{
				//			if (IsWithinBounds(x - 1, y + 1) && grid[x - 1][y + 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0, 0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else if (IsWithinBounds(x + 1, y + 1) && grid[x + 1][y + 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0, 0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else
				//			{
				//				meshActor->SetActorScale3D(FVector(1, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, 50, 0));
				//			}
				//		}
				//
				//		if (grid[x + 1][y] != ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR &&
				//			grid[x][y - 1] != ECellType::FLOOR && grid[x][y + 1] == ECellType::FLOOR)
				//		{
				//			if (IsWithinBounds(x - 1, y - 1) && grid[x - 1][y - 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0, 0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else if (IsWithinBounds(x + 1, y - 1) && grid[x + 1][y - 1] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0, 0,0));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//			else
				//			{
				//				meshActor->SetActorScale3D(FVector(1, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//			}
				//		}
				//
				//		if (IsWithinBounds(x + 1, y + 1) && IsWithinBounds(x - 1, y - 1) && IsWithinBounds(x + 1, y - 1) && IsWithinBounds(x - 1, y + 1))
				//		{
				//			// inner corners
				//			if (grid[x + 1][y + 1] == ECellType::FLOOR && grid[x + 1][y] != ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR && grid[x - 1][y - 1] != ECellType::FLOOR)
				//			{
				//				AStaticMeshActor* meshActor2 = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
				//				meshActor2->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_VoidMesh);
				//				meshActor2->SetMobility(EComponentMobility::Movable);
				//				meshActor->SetActorScale3D(FVector(1, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//				meshActor2->SetActorScale3D(FVector(0.75, 1, 1));
				//				meshActor2->GetStaticMeshComponent()->AddLocalOffset(FVector(-50, 0, 0));
				//				
				//			}
				//			
				//			if (grid[x - 1][y + 1] == ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR && grid[x + 1][y - 1] != ECellType::FLOOR)
				//			{
				//				AStaticMeshActor* meshActor2 = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
				//				meshActor2->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_VoidMesh);
				//				meshActor2->SetMobility(EComponentMobility::Movable);
				//				meshActor->SetActorScale3D(FVector(1, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, -50, 0));
				//				meshActor2->SetActorScale3D(FVector(0.75, 1, 1));
				//				meshActor2->GetStaticMeshComponent()->AddLocalOffset(FVector(50, 0, 0));
				//				
				//			}
				//			
				//			if (grid[x + 1][y - 1] == ECellType::FLOOR && grid[x + 1][y] != ECellType::FLOOR && grid[x][y - 1] != ECellType::FLOOR && grid[x - 1][y + 1] != ECellType::FLOOR)
				//			{
				//				AStaticMeshActor* meshActor2 = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
				//				meshActor2->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_VoidMesh);
				//				meshActor2->SetMobility(EComponentMobility::Movable);
				//				meshActor->SetActorScale3D(FVector(1, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, 50, 0));
				//				meshActor2->SetActorScale3D(FVector(0.75, 1, 1));
				//				meshActor2->GetStaticMeshComponent()->AddLocalOffset(FVector(-50, 0, 0));
				//				
				//			}
				//			
				//			if (grid[x - 1][y - 1] == ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR && grid[x][y - 1] != ECellType::FLOOR && grid[x + 1][y + 1] != ECellType::FLOOR)
				//			{
				//				AStaticMeshActor* meshActor2 = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
				//				meshActor2->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_VoidMesh);
				//				meshActor2->SetMobility(EComponentMobility::Movable);
				//				meshActor->SetActorScale3D(FVector(1, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(0, 50, 0));
				//				meshActor2->SetActorScale3D(FVector(0.75, 1, 1));
				//				meshActor2->GetStaticMeshComponent()->AddLocalOffset(FVector(50, 0, 0));
				//				
				//			}
				//
				//			// outer corners
				//			if (grid[x][y - 1] == ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR && grid[x - 1][y] == ECellType::FLOOR && 
				//				grid[x + 1][y] != ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0.75, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(50, 50, 0));
				//				
				//			}
				//
				//			if (grid[x][y - 1] == ECellType::FLOOR && grid[x][y + 1] != ECellType::FLOOR && grid[x - 1][y] != ECellType::FLOOR && 
				//				grid[x + 1][y] == ECellType::FLOOR)
				//			{
				//				meshActor->SetActorScale3D(FVector(0.75, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(-50, 50, 0));
				//				
				//			}
				//
				//			if (grid[x][y + 1] == ECellType::FLOOR && grid[x][y - 1] != ECellType::FLOOR && grid[x + 1][y] == ECellType::FLOOR && 
				//				grid[x - 1][y] != ECellType::FLOOR )
				//			{
				//				meshActor->SetActorScale3D(FVector(0.75, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(-50, -50, 0));
				//				
				//			}
				//
				//			if (grid[x][y + 1] == ECellType::FLOOR && grid[x][y - 1] != ECellType::FLOOR && grid[x - 1][y] == ECellType::FLOOR && 
				//				grid[x + 1][y] != ECellType::FLOOR )
				//			{
				//				meshActor->SetActorScale3D(FVector(0.75, 0.75, 1));
				//				meshActor->GetStaticMeshComponent()->AddLocalOffset(FVector(50, -50, 0));
				//				
				//			}
				//		}
				//	}
				//}
			}
			else if (m_Data->m_DungeonGrid[x][y] == ECellType::FLOOR)
			{
				pos.X += (float)posOffset.X;
				pos.Y += (float)posOffset.Y;
				pos.Z += (float)posOffset.Z;
				FActorSpawnParameters params;
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params));
				//AStaticMeshActor* meshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params); // testing!
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
					meshActor->GetStaticMeshComponent()->SetStaticMesh(m_DungeonTheme->m_FloorMeshes[0].m_Mesh);
					meshActor->SetMobility(EComponentMobility::Static);
				}

				//TSubclassOf<ADungeonMesh> enemyToSpawn = m_DungeonTheme->m_Test;
				//
				//if (enemyToSpawn)
				//{
				//	GetWorld()->SpawnActor<ADungeonMesh>(enemyToSpawn, pos, { 0, 0, 0 });
				//}
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
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params));
					//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 0, 0 }); // testing!
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
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0, 180, 0 }, params));
					//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 180, 0 }); // testing!
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
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params));
					//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 0, 0 }); // testing!
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
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0, -90, 0 }, params));
					//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, -90, 0 }); // testing!
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
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0, 90, 0 }, params));
					//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0, 90, 0 }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 0 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 0 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, -90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, -90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 180 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 180 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 0 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 0 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 180 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, 180 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
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
				ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
				AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, -90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }, params));
				//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, { 0 + (float)rotOffset.X, -90 + (float)rotOffset.Y, 0 + (float)rotOffset.Z }); // testing!
				if (meshActor)
				{
					meshActor->SetMobility(EComponentMobility::Movable);
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
			case ECellType::WALLCONVEX:  Ch = TEXT('O'); break;
			case ECellType::WALLCONCAVE:   Ch = TEXT('C'); break;
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
			FActorSpawnParameters params;
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

void UDungeonBuilder::BuildBossRoom()
{
	for (int i = 0; i < m_Data->m_AllRooms.Num(); i++)
	{
		ERoomType curType = m_Data->m_AllRooms[i].m_RoomType;

		if (curType == ERoomType::EXIT)
		{
			for (int x = m_Data->m_AllRooms[i].m_RoomOrigin.X; x < m_Data->m_AllRooms[i].m_RoomOrigin.X + m_Data->m_AllRooms[i].m_RoomCellLength; x++)
			{
				for (int y = m_Data->m_AllRooms[i].m_RoomOrigin.Y; y < m_Data->m_AllRooms[i].m_RoomOrigin.Y + m_Data->m_AllRooms[i].m_RoomCellWidth; y++)
				{
					FVector pos = { static_cast<float>(x * m_UnitSize), static_cast<float>(y * m_UnitSize), 0.0f };
					FInt32Vector posOffset = m_DungeonTheme->m_FloorPosOffset;
					TArray<TArray<ECellType>> grid = m_Data->m_DungeonGrid;
					pos.X += (float)posOffset.X;
					pos.Y += (float)posOffset.Y;
					pos.Z += (float)posOffset.Z + m_DungeonTheme->m_BossFloorZOffset;
					FActorSpawnParameters params;
					ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
					AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(chunkManager->SpawnActorInChunk(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator, params));
					//AStaticMeshActor* meshActor = m_WorldContext->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), pos, FRotator::ZeroRotator); // testing!
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
		FVector position = { (float)intPos.X * m_UnitSize, (float)intPos.Y * m_UnitSize, 0 };

		int32 randIndex = FMath::RandRange(0, m_DungeonTheme->m_EnemyCharacters.Num() - 1);
		TSubclassOf<AEnemyCharacter> enemyToSpawn = m_DungeonTheme->m_EnemyCharacters[randIndex];

		if (enemyToSpawn)
		{
			//GetWorld()->SpawnActor<AEnemyCharacter>(enemyToSpawn, position, {0, 0, 0});
			FActorSpawnParameters params;
			ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
			chunkManager->SpawnActorInChunk(enemyToSpawn, position, FRotator::ZeroRotator, params);
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
