// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_OptionsNPC.h"

void UWidget_OptionsNPC::NativeConstruct()
{
	if (Back)
	{
		Back->OnClicked.RemoveDynamic(this, &UWidget_OptionsNPC::OnButtonClicked);
		Back->OnClicked.AddDynamic(this, &UWidget_OptionsNPC::OnButtonClicked);
	}
}

void UWidget_OptionsNPC::OnButtonClicked()
{
	RemoveFromParent();

	FInputModeGameOnly InputMode;
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetInputMode(InputMode);
}