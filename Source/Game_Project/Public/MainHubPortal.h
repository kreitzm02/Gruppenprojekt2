// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "Engine/LevelStreamingDynamic.h"
#include "MainHubPortal.generated.h"


UCLASS()
class GAME_PROJECT_API AMainHubPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainHubPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* m_BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_Mesh;


	ULevelStreamingDynamic* NewLevel;
	void Load();
};
