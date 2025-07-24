// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

private:
	UPROPERTY(EditAnywhere, Category = "NPC Settings", meta = (DisplayName = "Skeletal Mesh"))
	USkeletalMesh* m_NPCMesh;

	UPROPERTY(EditAnywhere, Category = "NPC Settings", meta = (DisplayName = "Idle Animation"))
	UAnimSequence* m_IdleAnimation;

	UPROPERTY(EditAnywhere, Category = "NPC Settings", meta = (DisplayName = "Interact Animation"))
	UAnimSequence* m_InteractAnimation;

	UPROPERTY(EditAnywhere, Category = "NPC Settings", meta = (DisplayName = "Random Animation"))
	UAnimSequence* m_AltAnimation;

	bool IsInteractedWith();
	void PlayInteraction();

	FTimerHandle m_RandAnimSwitchHandle;

	double m_TimeToSwitchRandAnim;
	UAnimSequence* m_CurrentAnim;
	USkeletalMeshComponent* m_MeshComp;
};
