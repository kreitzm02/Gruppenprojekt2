// Fill out your copyright notice in the Description page of Project Settings.


#include "WMainHubClassNPC.h"
#include <Player/PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void UWMainHubClassNPC::NativeConstruct()
{
	if (Button_Class1)
	{
		Button_Class1->OnClicked.RemoveDynamic(this, &UWMainHubClassNPC::OnButton1Clicked);
		Button_Class1->OnClicked.AddDynamic(this, &UWMainHubClassNPC::OnButton1Clicked);
		Button_Class1->SetUserFocus(GetWorld()->GetFirstPlayerController());
	}

	if (Button_Class2)
	{
		Button_Class2->OnClicked.RemoveDynamic(this, &UWMainHubClassNPC::OnButton2Clicked);
		Button_Class2->OnClicked.AddDynamic(this, &UWMainHubClassNPC::OnButton2Clicked);
	}

	if (Button_Class3)
	{
		Button_Class3->OnClicked.RemoveDynamic(this, &UWMainHubClassNPC::OnButton3Clicked);
		Button_Class3->OnClicked.AddDynamic(this, &UWMainHubClassNPC::OnButton3Clicked);
	}
}

void UWMainHubClassNPC::OnButton1Clicked()
{
	RemoveFromParent();
	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->ChangeToPlayerClassA();
		}
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void UWMainHubClassNPC::OnButton2Clicked()
{
	RemoveFromParent();
	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->ChangeToPlayerClassB();
		}
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void UWMainHubClassNPC::OnButton3Clicked()
{
	RemoveFromParent();
	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->ChangeToPlayerClassC();
		}
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
