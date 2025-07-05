// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/LevelStreamingDynamic.h>
#include "DungeonLevelStreamer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_PROJECT_API UDungeonLevelStreamer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDungeonLevelStreamer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Dungeon")
	TSoftObjectPtr<UWorld> m_DungeonLevelInstance;

	UPROPERTY(EditAnywhere, Category = "Dungeon")
	FVector m_LevelOffset = FVector(0.0f, 0.0f, 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void LoadDungeon();

private:

	UPROPERTY()
	ULevelStreamingDynamic* m_LoadedDungeonInstance = nullptr;
		
};
