// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Stats", meta = (DisplayName = "Health Points"))
	int m_HP;

	UPROPERTY(EditAnywhere, Category = "Stats", meta = (DisplayName = "Attack Points"))
	int m_AP;

	UPROPERTY(EditAnywhere, Category = "Stats", meta = (DisplayName = "Defense Points"))
	int m_DP;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_MeshComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_MeshComp2;
};
