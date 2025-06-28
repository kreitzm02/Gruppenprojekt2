// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_ProjectileBase.h"
#include "Enemy_RogueArrow.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AEnemy_RogueArrow : public AEnemy_ProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy_RogueArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void MoveInDirection(float a_deltaTime) override;
};
