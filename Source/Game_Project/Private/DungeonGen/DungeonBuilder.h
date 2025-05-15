// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DungeonData.h"
#include "DungeonTheme.h"
#include "DungeonBuilder.generated.h"

/**
 * 
 */
UCLASS()
class UDungeonBuilder : public UObject
{
	GENERATED_BODY()

	float m_UnitSize;
	UDungeonTheme* m_DungeonTheme;
	FDungeonData* m_Data;
	UWorld* m_WorldContext;

public:
	void Init(float a_UnitSize, UDungeonTheme* a_Theme, FDungeonData* a_Data, UWorld* a_World);
	void BuildFloor();
};
