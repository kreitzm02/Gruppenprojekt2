// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_GameTimer.h"
#include "GameFramework/Actor.h"
#include "GameTimer.generated.h"

UCLASS()
class GAME_PROJECT_API AGameTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnTimerFinished();

	FTimerHandle m_levelTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int m_maxGameTime = 60;

	float m_remainingTime;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FName m_endbossLevelName;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> m_timerWidget = nullptr;

	UPROPERTY()
	class UWidget_GameTimer* m_timerWidgetInstance = nullptr;
};
