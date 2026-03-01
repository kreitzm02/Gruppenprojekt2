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
	//UGameplayStatics::OpenLevel(this, FName("TutorialArea1"));

	if (GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->HasAuthority())
	{
		DoServerTravel();
		UE_LOG(LogTemp, Warning, TEXT("Has Authority"));
	}
	else
	{
		RequestServerTravel();
		UE_LOG(LogTemp, Warning, TEXT("Has no Authority"));
	}
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

void UWMainHubTutorialNPC::RequestServerTravel_Implementation()
{
	DoServerTravel();
}

void UWMainHubTutorialNPC::DoServerTravel()
{
	
	if (!GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->HasAuthority())
	{
		return;
	}
	if (m_travelInProgress)
	{
		return;
	}
	m_travelInProgress = true;
	if (UWorld* world = GetWorld())
	{
		const ENetMode NetMode = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->GetNetMode();
		UE_LOG(LogTemp, Warning, TEXT("TRAVEL: NetMode=%d (0=Standalone,1=Dedicated,2=Listen,3=Client) HasAuthority=%d"),
			(int32)NetMode, GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->HasAuthority() ? 1 : 0);
		world->ServerTravel("/Game/TutorialArea1?listen");
	}
}