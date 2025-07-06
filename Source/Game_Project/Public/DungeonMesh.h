// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonMesh.generated.h"

UCLASS()
class GAME_PROJECT_API ADungeonMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonMesh();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_MeshComp;

	UPROPERTY(EditAnywhere)
	UStaticMesh* m_DefaultMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
