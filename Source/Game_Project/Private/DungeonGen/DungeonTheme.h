// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DungeonMeshEntry.h"
#include "EnemyCharacter.h"
#include <DungeonMesh.h>
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

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Wall Inner Corner Mesh"))
	UStaticMesh* m_WallCornerMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Wall Outer Corner Mesh"))
	UStaticMesh* m_WallOuterCornerMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Torch Mesh"))
	TSubclassOf<AStaticMeshActor> m_TorchMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Boss Floor Mesh"))
	UStaticMesh* m_BossFloorMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Chest Mesh"))
	USkeletalMesh* m_ChestMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "O Corner Wall Mesh"))
	UStaticMesh* m_WallOCornerMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "U Corner Wall Mesh"))
	UStaticMesh* m_WallUCornerMesh;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Decoration Meshes A"))
	TArray <FDungeonMeshEntry> m_DecorationVarA;

	UPROPERTY(EditAnywhere, Category = "Meshes", meta = (DisplayName = "Decoration Meshes B"))
	TArray <FDungeonMeshEntry> m_DecorationVarB;

	UPROPERTY(EditAnywhere, Category = "Debug Only", meta = (DisplayName = "Start Cube"))
	UStaticMesh* m_StartCubeMesh;

	UPROPERTY(EditAnywhere, Category = "Debug Only", meta = (DisplayName = "Exit Cube"))
	UStaticMesh* m_ExitCubeMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh Rotation and Position", meta = (DisplayName = "Floor Position Offset"))
	FInt32Vector m_FloorPosOffset;

	UPROPERTY(EditAnywhere, Category = "Mesh Rotation and Position", meta = (DisplayName = "Wall Rotation Offset"))
	FInt32Vector m_WallRotOffset;

	UPROPERTY(EditAnywhere, Category = "Mesh Rotation and Position", meta = (DisplayName = "Wall Inner Corner Rotation Offset"))
	FInt32Vector m_WallCornerRotOffset;

	UPROPERTY(EditAnywhere, Category = "Mesh Rotation and Position", meta = (DisplayName = "Wall Outer Corner Rotation Offset"))
	FInt32Vector m_WallCornerORotOffset;

	UPROPERTY(EditAnywhere, Category = "Mesh Rotation and Position", meta = (DisplayName = "Wall Outer Corner Position Offset"))
	FInt32Vector m_WallCornerOPosOffset;

	UPROPERTY(EditAnywhere, Category = "Mesh Rotation and Position", meta = (DisplayName = "Wall O Corner Position Offset"))
	FInt32Vector m_WallOPosOffset;

	UPROPERTY(EditAnywhere, Category = "Mesh Rotation and Position", meta = (DisplayName = "Boss Floor z Offset"))
	int32 m_BossFloorZOffset;

	UPROPERTY(EditAnywhere, Category = "Enemies", meta = (DisplayName = "Default Spawnable Enemies"))
	TArray<TSubclassOf<class AEnemyCharacter>> m_EnemyCharacters;

	UPROPERTY(EditAnywhere, Category = "Enemies", meta = (DisplayName = "Spawnable Boss Enemies"))
	TArray<TSubclassOf<class AEnemyCharacter>> m_BossCharacters;
};
