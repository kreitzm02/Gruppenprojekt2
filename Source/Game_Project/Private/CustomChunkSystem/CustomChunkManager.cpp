// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomChunkSystem/CustomChunkManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ACustomChunkManager::ACustomChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AActor* ACustomChunkManager::SpawnActorInChunk(UClass* a_ActorClass, const FVector& a_Position, const FRotator& a_Rotation, const FActorSpawnParameters& a_SpawnParameters)
{
	AActor* newActor = GetWorld()->SpawnActor<AActor>(a_ActorClass, a_Position, a_Rotation, a_SpawnParameters);
	if (!newActor) return nullptr;

	FIntPoint chunkPosition = GetChunkPosFromWorldPos(a_Position);

	if (!m_AllChunks.Contains(chunkPosition)) m_AllChunks.Add(chunkPosition, FCustomChunk(chunkPosition.X, chunkPosition.Y));

	FCustomChunk& chunk = m_AllChunks[chunkPosition];
	chunk.m_AllActorsInThisChunk.Add(newActor);

	return newActor;
}

// Called when the game starts or when spawned
void ACustomChunkManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomChunkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateChunkActivation();
}

FIntPoint ACustomChunkManager::GetChunkPosFromWorldPos(const FVector& a_WorldPos) const
{
	int32 x = FMath::FloorToInt(a_WorldPos.X / m_ChunkSize);
	int32 y = FMath::FloorToInt(a_WorldPos.Y / m_ChunkSize);
	return FIntPoint(x, y);
}

void ACustomChunkManager::UpdateChunkActivation()
{
	APawn* player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!player) return;

	FIntPoint playerChunk = GetChunkPosFromWorldPos(player->GetActorLocation());

	for (auto& pair : m_AllChunks)
	{
		const FIntPoint& pos = pair.Key;
		bool shouldBeActive = FMath::Abs(pos.X - playerChunk.X) <= m_ChunkActiveRadius && FMath::Abs(pos.Y - playerChunk.Y) <= m_ChunkActiveRadius;

		SetChunkActive(pos, shouldBeActive);
	}
}

void ACustomChunkManager::SetChunkActive(const FIntPoint& a_Position, bool a_Active)
{
	FCustomChunk* chunk = m_AllChunks.Find(a_Position);
	if (!chunk) return;

	for (int i = 0; i < chunk->m_AllActorsInThisChunk.Num(); i++)
	{
		if (AActor* actor = chunk->m_AllActorsInThisChunk[i].Get())
		{
			actor->SetActorTickEnabled(a_Active);
			actor->SetActorHiddenInGame(!a_Active);
			actor->SetActorEnableCollision(a_Active);
			
			TArray<UPrimitiveComponent*> comps;
			actor->GetComponents<UPrimitiveComponent>(comps);
			for (auto* c : comps)
			{
				if (c->IsSimulatingPhysics())
					c->SetSimulatePhysics(a_Active);
			}
		}
	}
}

