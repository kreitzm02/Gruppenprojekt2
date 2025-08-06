// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "DungeonDoorBehaviour.generated.h"


UCLASS()
class GAME_PROJECT_API ADungeonDoorBehaviour : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonDoorBehaviour();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    bool m_ShouldOpen = false;
private:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* m_DoorMesh;

    UPROPERTY(EditAnywhere)
    UBoxComponent* m_TriggerBox;

    FVector m_ClosedPosition;

    float m_MoveSpeed = 200.0f;

    UFUNCTION()
    void OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

};
