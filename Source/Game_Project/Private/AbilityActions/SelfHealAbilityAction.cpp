// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/SelfHealAbilityAction.h"
#include <Player/PlayerCharacter.h>
#include <NiagaraFunctionLibrary.h>
#include <Game_GameInstance.h>

void USelfHealAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(GetWeaponMeshName(m_WeaponMeshName));

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

void USelfHealAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (!m_HealVFX || !a_AbilityUser) return;

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &USelfHealAbilityAction::PlaySelfHeal, a_AbilityUser), m_StartingDelay, false);
}

void USelfHealAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_MoveTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_StartTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Damage Circle Ability Action Ended"));
}

void USelfHealAbilityAction::PlaySelfHeal(AActor* a_AbilityUser)
{
	// niagara
	m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_HealVFX, a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f), 
		FRotator::ZeroRotator, FVector(1.0f), true, true, ENCPoolMethod::None, true);

	// heal player
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->TryAddPlayerHealth(m_HPAmount * Cast<UGame_GameInstance>(a_AbilityUser->GetWorld()->GetGameInstance())->m_playerSave->GetPlayerHPRegenMultiplier());
	}

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_MoveTimerHandle, FTimerDelegate::CreateUObject(this, &USelfHealAbilityAction::MoveSelfHeal, a_AbilityUser), 0.01f, true);

	UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included SELF HEAL ability action!"))
}

void USelfHealAbilityAction::MoveSelfHeal(AActor* a_AbilityUser)
{
	m_VFXComp->SetWorldLocation(a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f));

	if (!IsValid(m_VFXComp)) EndAbilityAction(a_AbilityUser);
}
