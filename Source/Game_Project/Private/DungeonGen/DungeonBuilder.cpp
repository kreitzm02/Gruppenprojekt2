// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonBuilder.h"
#include "Engine/StaticMeshActor.h"

void UDungeonBuilder::Init(float a_UnitSize, UDungeonTheme* a_Theme, FDungeonData* a_Data, UWorld* a_World)
{
	m_UnitSize = a_UnitSize;
	m_DungeonTheme = a_Theme;
	m_Data = a_Data;
	m_WorldContext = a_World;
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
