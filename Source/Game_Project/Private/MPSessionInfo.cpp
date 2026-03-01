// Fill out your copyright notice in the Description page of Project Settings.


#include "MPSessionInfo.h"
#include "MultiplayerSubsystem.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMPSessionInfo::NativeConstruct()
{
	Super::NativeConstruct();
	//GetGameInstance()->GetSubsystem<UMultiplayerSubsystem>()->OnLobbyCodeCreated.AddDynamic(this, &UMPSessionInfo::DisplayLobbyCode);
}

TSharedRef<SWidget> UMPSessionInfo::RebuildWidget()
{
    // lässt das WidgetTree-Setup anlegen
    TSharedRef<SWidget> SuperWidget = Super::RebuildWidget();

    // Root (Canvas)
    RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
    WidgetTree->RootWidget = RootCanvas;

    // --- 1) TextBlock ---
    TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
    TitleText->SetText(FText::FromString(m_lobbyCode));

    RootCanvas->AddChild(TitleText);

    if (UCanvasPanelSlot* TextSlot = Cast<UCanvasPanelSlot>(TitleText->Slot))
    {
        // Position/Größe im Canvas (Pixel)
        TextSlot->SetPosition(FVector2D(50.f, 50.f));
        TextSlot->SetSize(FVector2D(600.f, 40.f));
        TextSlot->SetAutoSize(true); // passt Größe an Text an (optional)
    }

    // --- 2) Button mit Text ---
    ActionButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ActionButton"));
    RootCanvas->AddChild(ActionButton);

    if (UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(ActionButton->Slot))
    {
        ButtonSlot->SetPosition(FVector2D(50.f, 120.f));
        ButtonSlot->SetSize(FVector2D(260.f, 50.f));
    }

    // Button-Inhalt: TextBlock
    ButtonLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ButtonLabel"));
    ButtonLabel->SetText(FText::FromString(TEXT("Close")));
    ActionButton->AddChild(ButtonLabel);

    // Click-Event
    ActionButton->OnClicked.AddDynamic(this, &UMPSessionInfo::CloseUI);

    // wichtig: gib das RootWidget zurück (nicht SuperWidget)
    return WidgetTree->RootWidget->TakeWidget();
}

void UMPSessionInfo::DisplayLobbyCode(const FString& a_lobbyCode)
{
	m_lobbyCode = a_lobbyCode;
    TitleText->SetText(FText::FromString(m_lobbyCode));
}


void UMPSessionInfo::CloseUI()
{
	this->RemoveFromParent();
}
