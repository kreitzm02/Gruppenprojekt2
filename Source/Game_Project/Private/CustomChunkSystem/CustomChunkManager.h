// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomChunk.h"
#include "CustomChunkManager.generated.h"

UCLASS()
class ACustomChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomChunkManager();

	UPROPERTY(EditAnywhere, Category="Chunk", meta=(ClampMin = "100", UIMin = "100", ClampMax = "1000000", UIMax = "1000000", DisplayName = "Chunk Size"))
	float m_ChunkSize = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Chunk", meta = (ClampMin = "1", UIMin = "1", ClampMax = "100", UIMax = "100", DisplayName = "Active Radius in Chunks"))
	int m_ChunkActiveRadius = 2;

	UPROPERTY(EditAnywhere, Category = "Chunk", meta = (ClampMin = "0.01", UIMin = "0.01", ClampMax = "1.0", UIMax = "1.0", DisplayName = "Time between Chunk updates"))
	float m_ChunkUpdateInterval = 0.2f;

	UPROPERTY()
	TMap<FIntPoint, FCustomChunk> m_AllChunks;

	AActor* SpawnActorInChunk(UClass* a_ActorClass, const FVector& a_Position, const FRotator& a_Rotation, const FActorSpawnParameters& a_SpawnParameters);
	AActor* SpawnActorInChunkDeferred(UClass* a_ActorClass, const FTransform& a_transform);
	AActor* FinishSpawningActorInChunk(AActor* Actor, const FTransform& SpawnTransform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FIntPoint GetChunkPosFromWorldPos(const FVector& a_WorldPos) const;
	void UpdateChunkActivation();
	void SetChunkActive(const FIntPoint& a_Position, bool a_Active);
	void UpdateChunkForMovableActors();

	float m_ChunkUpdateTimer = 0.0f;
	
};
