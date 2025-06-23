// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/MeleeAbilityAction.h"
#include <Player/PlayerCharacter.h>

FName UMeleeAbilityAction::GetWeaponMeshName() const
{
	switch (m_WeaponMeshName)
	{
		case EWeaponNames::MESH_CUSTOM:
			return m_CustomWeaponMeshName;
			break;
		case EWeaponNames::MESH_1H_AXE:
			return FName("1H_Axe");
			break;
		case EWeaponNames::MESH_1H_CROSSBOW:
			return FName("1H_Crossbow");
			break;
		case EWeaponNames::MESH_1H_DAGGER:
			return FName("1H_Dagger");
			break;
		case EWeaponNames::MESH_1H_SCYTHE:
			return FName("1H_Scythe");
			break;
		case EWeaponNames::MESH_1H_SWORD:
			return FName("1H_Sword");
			break;
		case EWeaponNames::MESH_1H_WAND:
			return FName("1H_Wand");
			break;
		case EWeaponNames::MESH_2H_MACE:
			return FName("2H_Mace");
			break;
		case EWeaponNames::MESH_2H_STAFF:
			return FName("2H_Staff");
			break;
		default:
			return FName("1H_Axe");
			break;
	}
}

void UMeleeAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(GetWeaponMeshName());
		player->ChangeAttackDamage(m_AdditionalDamage);

		if (m_AnimName == EAnimationNames::ANIM_1H_MELEE_ATTACK_STAB)
			m_AttackMontage = player->m_StabAttackMontage;
		else if (m_AnimName == EAnimationNames::ANIM_1H_MELEE_ATTACK_CHOP)
			m_AttackMontage = player->m_ChopAttackMontage;
		else if (m_AnimName == EAnimationNames::ANIM_1H_MELEE_ATTACK_SLICE_DIAGONAL)
			m_AttackMontage = player->m_SliceAttackMontage;
	}
}

void UMeleeAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->ClearAlreadyHitActors();
		player->m_AnimInstance->Montage_Play(m_AttackMontage);
		// sound
		player->ShowMeleeHitbox();
		UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included melee ability action!"))
	}
}
