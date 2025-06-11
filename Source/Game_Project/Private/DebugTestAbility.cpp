// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "DebugTestAbility.h"

UDebugTestAbility::UDebugTestAbility()
{
	m_AbilityName = FText::FromString("Debug Ability");
	m_CooldownTime = 5.0f;
}

void UDebugTestAbility::UseAbility(AActor* a_AbilityUser)
{
	Super::UseAbility(a_AbilityUser);

	UE_LOG(LogTemp, Log, TEXT("Debug Ability Was Used!"));
}

void UDebugTestAbility::EquipAbility(AActor* a_AbilityUser)
{
	Super::EquipAbility(a_AbilityUser);
	
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		UE_LOG(LogTemp, Log, TEXT("Debug Ability Was Equipped!"));
		player->HideAllWeapons();
	}
}
