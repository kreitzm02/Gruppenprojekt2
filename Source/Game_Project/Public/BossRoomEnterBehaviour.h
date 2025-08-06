// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "GameFramework/Actor.h"
#include <DungeonDoorBehaviour.h>
#include "BossRoomEnterBehaviour.generated.h"

UCLASS()
class GAME_PROJECT_API ABossRoomEnterBehaviour : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossRoomEnterBehaviour();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* m_TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* m_DungeonMusicContainer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> m_DungeonDoors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundWave* m_BossMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USoundWave* m_DungeonMusic;

	bool m_Overlapped = false;

};
