// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword1HAbility.h"
#include "Player/PlayerCharacter.h"
#include "Animation/AnimInstance.h"

USword1HAbility::USword1HAbility()
{
	m_AbilityName = FText::FromString("One-Handed Sword Ability");
	m_CooldownTime = 1.05f;
}

void USword1HAbility::UseAbility(AActor* a_AbilityUser)
{
	Super::UseAbility(a_AbilityUser);

	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->GetMesh()->PlayAnimation(player->m_PlayerCharDataAssets[0]->m_1HAttackVar1Anim, false);
	
		// damage kriegen methode vom target fehlt hier noch
	}
	else if (false)
	{
		// evtl version für die skeletons hier einfügen
	}
}

void USword1HAbility::EquipAbility(AActor* a_AbilityUser)
{
	Super::EquipAbility(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(FName("1H_Sword"));
	}
	else if (false)
	{
		// evtl version für die skeletons hier einfügen
	}
}
