// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include <Components/SphereComponent.h>
#include <Player/PlayerCharacter.h>
#include "NPC.generated.h"


UCLASS()
class GAME_PROJECT_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact();

	UPROPERTY(EditAnywhere, Category = "NPC")
	FText m_DisplayName;

	UPROPERTY(EditAnywhere, Category = "NPC")
	USkeletalMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere, Category = "NPC")
	UWidgetComponent* m_InteractionWidget;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_InteractionSphere;

	UPROPERTY()
	APlayerCharacter* m_OverlappingPlayer;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsPlayerInRange() const { return m_OverlappingPlayer != nullptr; }

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimSequence* m_IdleAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<UAnimSequence*> m_RandomAnims;

	FTimerHandle m_RandomAnimTimer;

	void TryPlayRandomAnim();
	
};
