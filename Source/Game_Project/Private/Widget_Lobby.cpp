// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Lobby.h"

#include "Kismet/GameplayStatics.h"

void UWidget_Lobby::StartGame()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetInputMode(FInputModeGameOnly());
	pc->bShowMouseCursor = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	RemoveFromParent();
	GetWorld()->ServerTravel("/Game/MainHub1?listen");
}
