// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonLevelStreamer.h"

// Sets default values for this component's properties
UDungeonLevelStreamer::UDungeonLevelStreamer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDungeonLevelStreamer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDungeonLevelStreamer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDungeonLevelStreamer::LoadDungeon()
{
    if (!m_DungeonLevelInstance.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("DungeonLevelAsset is not set!"));
        return;
    }

    FTransform transform;
    transform.SetLocation(GetOwner()->GetActorLocation() + m_LevelOffset);
    transform.SetRotation(FQuat::Identity);
    bool success = false;
    FString uniqueName = FString::Printf(TEXT("DungeonInstance_%s"), *FGuid::NewGuid().ToString());

    FLatentActionInfo latentInfo;
    latentInfo.CallbackTarget = this;

    m_LoadedDungeonInstance = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), m_DungeonLevelInstance, transform, success, uniqueName, ULevelStreamingDynamic::StaticClass(), false);

    if (success)
    {
        UE_LOG(LogTemp, Log, TEXT("Dungeon loaded: %s"), *uniqueName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Dungeon loading error"));
    }
}

