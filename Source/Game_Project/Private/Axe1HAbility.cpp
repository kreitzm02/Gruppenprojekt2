// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Axe1HAbility.h"

UAxe1HAbility::UAxe1HAbility()
{
	m_AbilityName = FText::FromString("One-Handed Axe Ability");
	m_CooldownTime = 1.5f;
}

void UAxe1HAbility::UseAbility(AActor* a_AbilityUser)
{
	Super::UseAbility(a_AbilityUser);

	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->GetMesh()->PlayAnimation(player->m_PlayerCharDataAssets[0]->m_1HAttackVar2Anim, false);
	
		// damage kriegen methode vom target fehlt hier noch
	}
	else if (false)
	{
		// evtl version für die skeletons hier einfügen
	}
}

void UAxe1HAbility::EquipAbility(AActor* a_AbilityUser)
{
	Super::EquipAbility(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(FName("1H_Axe"));
	}
	else if (false)
	{
		// evtl version für die skeletons hier einfügen
	}
}
