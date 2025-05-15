// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DungeonMeshEntry.h"
#include "DungeonTheme.generated.h"

UCLASS()
class GAME_PROJECT_API UDungeonTheme : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Floor Meshes"))
	TArray <FDungeonMeshEntry> m_FloorMeshes;
	
	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Wall Meshes"))
	TArray <FDungeonMeshEntry> m_WallMeshes;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Void Mesh"))
	UStaticMesh* m_VoidMesh;
	
	UPROPERTY(EditAnywhere, Category = "Meshes", meta= (DisplayName = "Wall Corner Mesh"))
	UStaticMesh* m_WallCornerMesh;
	
	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Door Mesh"))
	UStaticMesh* m_DoorMesh;
	
	UPROPERTY(EditAnywhere, Category = "Meshes", meta= (DisplayName = "Chest Mesh"))
	UStaticMesh* m_ChestMesh;
	
	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Decoration Meshes A"))
	TArray <FDungeonMeshEntry> m_DecorationVarA;
	
	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Decoration Meshes B"))
	TArray <FDungeonMeshEntry> m_DecorationVarB;
};
