// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialAreaNPC.h"

void ATutorialAreaNPC::Interact()
{
    if (m_UIClass && !m_UI)
    {
        m_UI = CreateWidget<UUserWidget>(GetWorld(), m_UIClass);
    }
    if (!m_UI) return;

    m_UI->AddToViewport();
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    //UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
}