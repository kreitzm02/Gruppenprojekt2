// Fill out your copyright notice in the Description page of Project Settings.


#include "WMainHubTutorialNPC.h"
#include "LoadingScreenManager.h"
#include <Kismet/GameplayStatics.h>

void UWMainHubTutorialNPC::NativeConstruct()
{
	if (Button_Yes)
	{
		Button_Yes->OnClicked.RemoveDynamic(this, &UWMainHubTutorialNPC::OnButton1Clicked);
		Button_Yes->OnClicked.AddDynamic(this, &UWMainHubTutorialNPC::OnButton1Clicked);
	}
	if (Button_No)
	{
		Button_No->OnClicked.RemoveDynamic(this, &UWMainHubTutorialNPC::OnButton2Clicked);
		Button_No->OnClicked.AddDynamic(this, &UWMainHubTutorialNPC::OnButton2Clicked);
		Button_No->SetUserFocus(GetWorld()->GetFirstPlayerController());
	}
}

void UWMainHubTutorialNPC::OnButton1Clicked()
{
	RemoveFromParent();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}
	ULoadingScreenManager::Get(GetWorld())->StartLoading(GetWorld());
	UGameplayStatics::OpenLevel(this, FName("TutorialArea1"));
}

void UWMainHubTutorialNPC::OnButton2Clicked()
{
	RemoveFromParent();
	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}