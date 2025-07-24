// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "BoundariesChecker.generated.h"

UCLASS()
class GAME_PROJECT_API ABoundariesChecker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoundariesChecker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOwnerEnemy(AEnemyCharacter* a_owner) { m_owner = a_owner; }

	void SetPlayer(ACharacter* a_player) { m_player = a_player; }

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool m_enableDebug = true;

	UPROPERTY()
	AEnemyCharacter* m_owner = nullptr;

	UPROPERTY()
	ACharacter* m_player = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_moveSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_lineTraceLength = 200.0f;

	FVector m_moveDirection = FVector::ZeroVector;

	float m_xSizeMin = 0.0f;
	float m_xSizeMax = 0.0f;
	float m_ySizeMin = 0.0f;
	float m_ySizeMax = 0.0f;

	int m_boundariesChecked = 0;
};
