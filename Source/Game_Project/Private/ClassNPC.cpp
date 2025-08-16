// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassNPC.h"
#include <Kismet/GameplayStatics.h>

void AClassNPC::Interact()
{
    if (m_UIClass && !m_UI)
    {
        m_UI = CreateWidget<UWMainHubClassNPC>(GetWorld(), m_UIClass);
    }
    if (!m_UI) return;

    m_UI->AddToViewport();
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
}
