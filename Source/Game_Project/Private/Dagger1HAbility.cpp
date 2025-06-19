// Fill out your copyright notice in the Description page of Project Settings.


#include "Dagger1HAbility.h"
#include <Player/PlayerCharacter.h>

UDagger1HAbility::UDagger1HAbility()
{
	m_AbilityName = FText::FromString("One-Handed Dagger Ability");
	m_CooldownTime = 0.8f;
}

void UDagger1HAbility::UseAbility(AActor* a_AbilityUser)
{
	Super::UseAbility(a_AbilityUser);

	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->GetMesh()->PlayAnimation(player->m_PlayerCharDataAssets[0]->m_1HAttackVar3Anim, false);
	
		// damage kriegen methode vom target fehlt hier noch
	}
	else if (false)
	{
		// evtl version für die skeletons hier einfügen
	}
}

void UDagger1HAbility::EquipAbility(AActor* a_AbilityUser)
{
	Super::EquipAbility(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(FName("1H_Dagger"));
	}
	else if (false)
	{
		// evtl version für die skeletons hier einfügen
	}
}
