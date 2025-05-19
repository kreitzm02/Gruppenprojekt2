// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonTheme.h"
#include "DungeonData.h"
#include "DungeonRoom.h"
#include "DungeonBuilder.h"
#include "DungeonGridUtils.h"
#include "DungeonGenUtils.h"
#include "DungeonGen.generated.h"

UCLASS()
class GAME_PROJECT_API ADungeonGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_DevCube;

private:
	const FString DEFAULT_CUBE_PATH = FString(TEXT("/Engine/BasicShapes/Cube.Cube"));
	
	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "Grid Length", ToolTip = "Amount of grid cells on the X axis."))
	int m_GridLength = 50;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "Grid Width", ToolTip = "Amount of grid cells on the Y axis."))
	int m_GridWidth = 50;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "Cell Size", ToolTip = "Size of a grid cell in cm.", 
		ClampMin = "100.0", ClampMax = "1600.0", UIMin = "100.0", UIMax = "1600.0"))
	float m_UnitSize = 400.0f;

	UPROPERTY(EditAnywhere, Category = "General Settings", meta = (DisplayName = "Generation Seed"))
	int m_Seed = -1;

	UPROPERTY(EditAnywhere, Category = "Room Settings", meta = (DisplayName = "Room Size (min)", 
		ClampMin = "2", ClampMax = "20", UIMin = "2", UIMax = "20"))
	int m_MinRoomSize = 2;

	UPROPERTY(EditAnywhere, Category = "Room Settings", meta = (DisplayName = "Room Size (max)",
		ClampMin = "2", ClampMax = "20", UIMin = "2", UIMax = "20"))
	int m_MaxRoomSize = 6;

	UPROPERTY(EditAnywhere, Category = "Room Settings", meta = (DisplayName = "Room Amount", 
		ToolTip = "The amount of tries to generate a room inside the grid. If overlaping is not allowed, the room amount will be lower than that."))
	int m_MaxRoomAmount = 50;

	UPROPERTY(EditAnywhere, Category = "Room Settings", meta = (DisplayName = "Allow Overlaping Rooms",
		ToolTip = "If allowed, rooms can be generated inside other rooms to create larger, more random looking rooms."))
	bool m_AllowOverlapingRooms = false;

	UPROPERTY(EditAnywhere, Category = "Dungeon Assets", meta = (DisplayName = "Dungeon Assets"))
	UDungeonTheme* m_DungeonTheme; 

	UPROPERTY(EditAnywhere, Category = "Dungeon Assets", meta = (DisplayName = "Wall Offset"))
	float m_WallOffset;
	FDungeonData m_Data;
	UDungeonBuilder* m_Builder;
	FRandomStream m_DungeonRng;

	void GenerateRooms();
	void GenerateCorridors();
	void GenerateDungeon();
};
