// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/MeleeAbilityAction.h"
#include <Player/PlayerCharacter.h>

void UMeleeAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(GetWeaponMeshName(m_WeaponMeshName));
		player->ChangeAttackDamage(m_AdditionalDamage);

		if (m_AnimName == EAAAnimationNames::ANIM_SPELLCAST_LONG)
			m_AttackMontage = player->m_LongSpellcastMontage;
		else if (m_AnimName == EAAAnimationNames::ANIM_SPELLCAST_RAISE)
			m_AttackMontage = player->m_RaiseSpellcastMontage;
		else if (m_AnimName == EAAAnimationNames::ANIM_SPELLCAST_SHOOT)
			m_AttackMontage = player->m_ShootSpellcastMontage;
		else if (m_AnimName == EAAAnimationNames::ANIM_1H_MELEE_ATTACK_STAB)
			m_AttackMontage = player->m_StabAttackMontage;
		else if (m_AnimName == EAAAnimationNames::ANIM_1H_MELEE_ATTACK_CHOP)
			m_AttackMontage = player->m_ChopAttackMontage;
		else if (m_AnimName == EAAAnimationNames::ANIM_1H_MELEE_ATTACK_SLICE_DIAGONAL)
			m_AttackMontage = player->m_SliceAttackMontage;
	}
}

void UMeleeAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->ClearAlreadyHitActors();
		player->ShowMeleeHitbox();
		player->m_AnimInstance->Montage_Play(m_AttackMontage);
		// sound
		UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included melee ability action!"))
	}
}

void UMeleeAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{

}
