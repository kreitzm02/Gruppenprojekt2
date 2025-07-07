// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomChunk.generated.h"

USTRUCT()
struct FCustomChunk
{
	GENERATED_BODY();

	UPROPERTY()
	FIntPoint m_ChunkWorldPos;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> m_AllActorsInThisChunk;

	FCustomChunk() : m_ChunkWorldPos(0, 0) {}
	FCustomChunk(int32 a_X, int32 a_Y) : m_ChunkWorldPos(a_X, a_Y) {}
};
