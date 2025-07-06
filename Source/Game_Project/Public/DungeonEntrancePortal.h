// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DungeonEntrancePortal.generated.h"

UCLASS()
class GAME_PROJECT_API ADungeonEntrancePortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonEntrancePortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* m_portalVFX = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDungeonAsCleared();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Portal Events")
	void SetPortalClearColor();

	UPROPERTY(VisibleAnywhere, Category = "Dungeon Streaming", meta = (AllowPrivateAccess = "true"))
	class UDungeonLevelStreamer* m_DungeonLevelStreamer;

private:
	UFUNCTION()
	void OnPortalEnter(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_portalTrigger = nullptr;

	bool m_dungeonCleared = false;
};
