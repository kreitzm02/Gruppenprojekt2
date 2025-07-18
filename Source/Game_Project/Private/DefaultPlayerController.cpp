// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include <Player/PlayerCharacter.h>

void ADefaultPlayerController::BeginPlay()
{
    Super::BeginPlay();

    APlayerCharacter* playerChar = Cast<APlayerCharacter>(GetPawn());
    if (playerChar && playerChar->GetMesh())
    {
        SetAudioListenerOverride(playerChar->GetMesh(), FVector::ZeroVector, FRotator::ZeroRotator);
    }
}
