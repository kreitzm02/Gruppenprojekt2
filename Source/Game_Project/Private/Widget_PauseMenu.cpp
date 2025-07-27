// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PauseMenu.h"
#include "Widget_OptionsMenu.h"
#include "Kismet/GameplayStatics.h"

void UWidget_PauseMenu::ResumeGame()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetInputMode(FInputModeGameOnly());
	pc->bShowMouseCursor = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UWidget_PauseMenu::OpenOptionsWidget()
{
	//open options
	if (!m_optionsMenu && m_widgetOptionsClass)
	{
		m_optionsMenu = CreateWidget<UWidget_OptionsMenu>(this, m_widgetOptionsClass);
	}

	if (m_optionsMenu)
	{
		RemoveFromParent();
		m_optionsMenu->SetPauseMenu(this);
		m_optionsMenu->AddToViewport();

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(m_optionsMenu->TakeWidget());
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		pc->SetInputMode(InputMode);
	}
}

void UWidget_PauseMenu::EndCurrentRound()
{
	//reset player stats
	//move to main hub
}

void UWidget_PauseMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
