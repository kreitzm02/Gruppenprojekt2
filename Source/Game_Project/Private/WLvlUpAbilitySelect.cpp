// Fill out your copyright notice in the Description page of Project Settings.


#include "WLvlUpAbilitySelect.h"
#include "Kismet/GameplayStatics.h"
#include <Player/PlayerCharacter.h>

void UWLvlUpAbilitySelect::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Ability1)
	{
		Button_Ability1->OnClicked.RemoveDynamic(this, &UWLvlUpAbilitySelect::OnButton1Clicked);
		Button_Ability1->OnClicked.AddDynamic(this, &UWLvlUpAbilitySelect::OnButton1Clicked);
		Button_Ability1->OnHovered.RemoveDynamic(this, &UWLvlUpAbilitySelect::OnButton1Hovered);
		Button_Ability1->OnHovered.AddDynamic(this, &UWLvlUpAbilitySelect::OnButton1Hovered);
		Button_Ability1->SetUserFocus(GetWorld()->GetFirstPlayerController());
	}
	if (Button_Ability2)
	{
		Button_Ability2->OnClicked.RemoveDynamic(this, &UWLvlUpAbilitySelect::OnButton2Clicked);
		Button_Ability2->OnClicked.AddDynamic(this, &UWLvlUpAbilitySelect::OnButton2Clicked);
		Button_Ability2->OnHovered.RemoveDynamic(this, &UWLvlUpAbilitySelect::OnButton2Hovered);
		Button_Ability2->OnHovered.AddDynamic(this, &UWLvlUpAbilitySelect::OnButton2Hovered);
	}
	if (Button_Ability3)
	{
		Button_Ability3->OnClicked.RemoveDynamic(this, &UWLvlUpAbilitySelect::OnButton3Clicked);
		Button_Ability3->OnClicked.AddDynamic(this, &UWLvlUpAbilitySelect::OnButton3Clicked);
		Button_Ability3->OnHovered.RemoveDynamic(this, &UWLvlUpAbilitySelect::OnButton3Hovered);
		Button_Ability3->OnHovered.AddDynamic(this, &UWLvlUpAbilitySelect::OnButton3Hovered);
	}
	if (Button_Skip)
	{
		Button_Skip->OnClicked.RemoveDynamic(this, &UWLvlUpAbilitySelect::OnButton4Clicked);
		Button_Skip->OnClicked.AddDynamic(this, &UWLvlUpAbilitySelect::OnButton4Clicked);
	}
}

void UWLvlUpAbilitySelect::SetButtonTexts(const FText& a_T1, const FText& a_T2, const FText& a_T3)
{
	if (Text_Ability1) Text_Ability1->SetText(a_T1);
	if (Text_Ability2) Text_Ability2->SetText(a_T2);
	if (Text_Ability3) Text_Ability3->SetText(a_T3);
}

void UWLvlUpAbilitySelect::SetButtonImages(UTexture2D* a_I1, UTexture2D* a_I2, UTexture2D* a_I3)
{
	if (Image_Ability1) Image_Ability1->Brush.SetResourceObject(a_I1);
	if (Image_Ability2) Image_Ability2->Brush.SetResourceObject(a_I2);
	if (Image_Ability3) Image_Ability3->Brush.SetResourceObject(a_I3);
}

void UWLvlUpAbilitySelect::SetLevelUpText(const FText& a_LvlUpMessage)
{
	if (Text_LevelMsg) Text_LevelMsg->SetText(a_LvlUpMessage);
}

void UWLvlUpAbilitySelect::SetDescriptionOnHovered(const FText& a_Name, const FText& a_Desc)
{
	if (Text_HoveredName) Text_HoveredName->SetText(a_Name);
	if (Text_HoveredDesc) Text_HoveredDesc->SetText(a_Desc);
}

void UWLvlUpAbilitySelect::OnButton1Clicked()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->AddAbilityFromUI(0);
		}
}

void UWLvlUpAbilitySelect::OnButton2Clicked()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->AddAbilityFromUI(1);
		}
}

void UWLvlUpAbilitySelect::OnButton3Clicked()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->AddAbilityFromUI(2);
		}
}

void UWLvlUpAbilitySelect::OnButton4Clicked()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc)
	{
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}
	RemoveFromViewport();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void UWLvlUpAbilitySelect::OnButton3Hovered()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			SetDescriptionOnHovered(Text_Ability3->GetText(), m_Descriptions[2]);
		}
}

void UWLvlUpAbilitySelect::OnButton2Hovered()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			SetDescriptionOnHovered(Text_Ability2->GetText(), m_Descriptions[1]);
		}
}

void UWLvlUpAbilitySelect::OnButton1Hovered()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			SetDescriptionOnHovered(Text_Ability1->GetText(), m_Descriptions[0]);
		}
}

void UWLvlUpAbilitySelect::SetDescriptions(const FText& a_D1, const FText& a_D2, const FText& a_D3)
{
	m_Descriptions.Empty();
	m_Descriptions.Add(a_D1);
	m_Descriptions.Add(a_D2);
	m_Descriptions.Add(a_D3);
}
