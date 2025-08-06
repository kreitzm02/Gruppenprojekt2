// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/ShieldAbilityAction.h"
#include <Player/PlayerCharacter.h>
#include <NiagaraFunctionLibrary.h>
#include <Game_GameInstance.h>

void UShieldAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
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

void UShieldAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (!m_ShieldVFX || !a_AbilityUser) return;

	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		m_SavedPlayerDP = player->GetPlayerDefense();

		player->ChangeDefense(m_SavedPlayerDP + m_DPAmount);
	}

	m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_ShieldVFX, a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f),
		FRotator::ZeroRotator, FVector(1.0f), false, true, ENCPoolMethod::None, true);

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UShieldAbilityAction::PlayShield, a_AbilityUser), m_StartingDelay, false);
}

void UShieldAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_MoveTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_StartTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_EndTimerHandle);
	m_VFXComp->DestroyComponent();
	m_VFXComp = nullptr;

	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->ChangeDefense(m_SavedPlayerDP);
	}
	UE_LOG(LogTemp, Warning, TEXT("Shield Ability Action Ended"));
}

void UShieldAbilityAction::PlayShield(AActor* a_AbilityUser)
{
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_MoveTimerHandle, FTimerDelegate::CreateUObject(this, &UShieldAbilityAction::MoveShield, a_AbilityUser), 0.01f, true);

	UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included berserker ability action!"))

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_EndTimerHandle, FTimerDelegate::CreateUObject(this, &UShieldAbilityAction::EndAbilityAction, a_AbilityUser), m_Duration, false);
}

void UShieldAbilityAction::MoveShield(AActor* a_AbilityUser)
{
	m_VFXComp->SetWorldLocation(a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f));
}
