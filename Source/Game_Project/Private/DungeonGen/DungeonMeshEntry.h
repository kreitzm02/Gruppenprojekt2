// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonMeshEntry.generated.h"

USTRUCT()
struct FDungeonMeshEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* m_Mesh;

	UPROPERTY(EditAnywhere)
	int m_Weight;
};

