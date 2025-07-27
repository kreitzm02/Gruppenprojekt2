// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_OptionsMenu.h"
#include "Widget_PauseMenu.h"

void UWidget_OptionsMenu::BackToPauseMenu()
{
	RemoveFromParent();
	m_pauseMenu->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(m_pauseMenu->TakeWidget());
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetInputMode(InputMode);
}
