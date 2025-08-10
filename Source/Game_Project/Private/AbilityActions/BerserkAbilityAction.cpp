// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/BerserkAbilityAction.h"
#include <Player/PlayerCharacter.h>
#include <NiagaraFunctionLibrary.h>
#include <Game_GameInstance.h>

void UBerserkAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
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

void UBerserkAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (!m_BerserkVFX || !a_AbilityUser) return;
	UGame_GameInstance* gi = Cast<UGame_GameInstance>(a_AbilityUser->GetWorld()->GetGameInstance());
	if (gi->m_AdditionalDamage < m_APAmount)
	{
		gi->m_AdditionalDamage += m_APAmount - gi->m_AdditionalDamage;
	}
		
	m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_BerserkVFX, a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f),
		FRotator::ZeroRotator, FVector(1.0f), false, true, ENCPoolMethod::None, true);

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UBerserkAbilityAction::PlayBerserk, a_AbilityUser), m_StartingDelay, false);
}

void UBerserkAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_MoveTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_StartTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_DamageTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_EndTimerHandle);
	m_VFXComp->DestroyComponent();
	m_VFXComp = nullptr;

	UGame_GameInstance* gi = Cast<UGame_GameInstance>(a_AbilityUser->GetWorld()->GetGameInstance());
	if (gi->m_AdditionalDamage == m_APAmount)
	{
		gi->m_AdditionalDamage = 0;
	}
	UE_LOG(LogTemp, Warning, TEXT("berserker Ability Action Ended"));
}

void UBerserkAbilityAction::PlayBerserk(AActor* a_AbilityUser)
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		m_SavedPlayerHP = player->GetPlayerHealth();
	}
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_MoveTimerHandle, FTimerDelegate::CreateUObject(this, &UBerserkAbilityAction::MoveBerserk, a_AbilityUser), 0.01f, true);

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_DamageTimerHandle, FTimerDelegate::CreateUObject(this, &UBerserkAbilityAction::GettingDamage, a_AbilityUser), 1.0f, true);

	UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included berserker ability action!"))

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_EndTimerHandle, FTimerDelegate::CreateUObject(this, &UBerserkAbilityAction::EndAbilityAction, a_AbilityUser), m_Duration, false);
}

void UBerserkAbilityAction::MoveBerserk(AActor* a_AbilityUser)
{
	m_VFXComp->SetWorldLocation(a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f));
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		if (player->GetPlayerHealth() > m_SavedPlayerHP)
		{
			player->TryAddPlayerHealth((player->GetPlayerHealth() - m_SavedPlayerHP) * -1.0f);
		}
		m_SavedPlayerHP = player->GetPlayerHealth();
	}
}

void UBerserkAbilityAction::GettingDamage(AActor* a_AbilityUser)
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->TryAddPlayerHealth(-m_PlayerDamagePerSecond);
	}
}
