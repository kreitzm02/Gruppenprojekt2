// Fill out your copyright notice in the Description page of Project Settings.


#include "WLvlUpAbilityReplace.h"
#include <Player/PlayerCharacter.h>

void UWLvlUpAbilityReplace::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Ability1)
	{
		Button_Ability1->OnClicked.RemoveDynamic(this, &UWLvlUpAbilityReplace::OnButton1Clicked);
		Button_Ability1->OnClicked.AddDynamic(this, &UWLvlUpAbilityReplace::OnButton1Clicked);
	}
	if (Button_Ability2)
	{
		Button_Ability2->OnClicked.RemoveDynamic(this, &UWLvlUpAbilityReplace::OnButton2Clicked);
		Button_Ability2->OnClicked.AddDynamic(this, &UWLvlUpAbilityReplace::OnButton2Clicked);
	}
	if (Button_Ability3)
	{
		Button_Ability3->OnClicked.RemoveDynamic(this, &UWLvlUpAbilityReplace::OnButton3Clicked);
		Button_Ability3->OnClicked.AddDynamic(this, &UWLvlUpAbilityReplace::OnButton3Clicked);
	}
	if (Button_Ability4)
	{
		Button_Ability4->OnClicked.RemoveDynamic(this, &UWLvlUpAbilityReplace::OnButton4Clicked);
		Button_Ability4->OnClicked.AddDynamic(this, &UWLvlUpAbilityReplace::OnButton4Clicked);
	}
}

void UWLvlUpAbilityReplace::SetButtonTexts(const FText& a_T1, const FText& a_T2, const FText& a_T3, const FText& a_T4)
{
	if (Text_Ability1) Text_Ability1->SetText(a_T1);
	if (Text_Ability2) Text_Ability2->SetText(a_T2);
	if (Text_Ability3) Text_Ability3->SetText(a_T3);
	if (Text_Ability4) Text_Ability4->SetText(a_T4);
}

void UWLvlUpAbilityReplace::OnButton1Clicked()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->ReplaceAbilityFromUI(0);
		}
}

void UWLvlUpAbilityReplace::OnButton2Clicked()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->ReplaceAbilityFromUI(1);
		}
}

void UWLvlUpAbilityReplace::OnButton3Clicked()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->ReplaceAbilityFromUI(2);
		}
}

void UWLvlUpAbilityReplace::OnButton4Clicked()
{
	if (APlayerController* pc = GetOwningPlayer())
		if (APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetPawn()))
		{
			player->ReplaceAbilityFromUI(3);
		}
}