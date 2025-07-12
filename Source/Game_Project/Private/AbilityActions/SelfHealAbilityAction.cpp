// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/SelfHealAbilityAction.h"
#include <Player/PlayerCharacter.h>
#include <NiagaraFunctionLibrary.h>

void USelfHealAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(GetWeaponMeshName());

		if (m_AnimName == ESelfHealAnimationNames::ANIM_SPELLCAST_LONG)
			m_AttackMontage = player->m_LongSpellcastMontage;
		else if (m_AnimName == ESelfHealAnimationNames::ANIM_SPELLCAST_RAISE)
			m_AttackMontage = player->m_RaiseSpellcastMontage;
		else if (m_AnimName == ESelfHealAnimationNames::ANIM_SPELLCAST_SHOOT)
			m_AttackMontage = player->m_ShootSpellcastMontage;
	}
}

void USelfHealAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (!m_HealVFX || !a_AbilityUser) return;

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &USelfHealAbilityAction::PlaySelfHeal, a_AbilityUser), m_StartingDelay, false);
}

FName USelfHealAbilityAction::GetWeaponMeshName() const
{
	switch (m_WeaponMeshName)
	{
	case ESelfHealWeaponNames::MESH_CUSTOM:
		return m_CustomWeaponMeshName;
		break;
	case ESelfHealWeaponNames::MESH_1H_AXE:
		return FName("1H_Axe");
		break;
	case ESelfHealWeaponNames::MESH_1H_CROSSBOW:
		return FName("1H_Crossbow");
		break;
	case ESelfHealWeaponNames::MESH_1H_DAGGER:
		return FName("1H_Dagger");
		break;
	case ESelfHealWeaponNames::MESH_1H_SCYTHE:
		return FName("1H_Scythe");
		break;
	case ESelfHealWeaponNames::MESH_1H_SWORD:
		return FName("1H_Sword");
		break;
	case ESelfHealWeaponNames::MESH_1H_WAND:
		return FName("1H_Wand");
		break;
	case ESelfHealWeaponNames::MESH_2H_MACE:
		return FName("2H_Mace");
		break;
	case ESelfHealWeaponNames::MESH_2H_STAFF:
		return FName("2H_Staff");
		break;
	default:
		return FName("1H_Axe");
		break;
	}
}

void USelfHealAbilityAction::PlaySelfHeal(AActor* a_AbilityUser)
{
	// niagara
	m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_HealVFX, a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f), 
		FRotator::ZeroRotator, FVector(1.0f), true, true, ENCPoolMethod::None, true);

	// heal player
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->TryAddPlayerHealth(m_HPAmount);
	}

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_MoveTimerHandle, FTimerDelegate::CreateUObject(this, &USelfHealAbilityAction::MoveSelfHeal, a_AbilityUser), 0.01f, true);

	UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included SELF HEAL ability action!"))
}

void USelfHealAbilityAction::MoveSelfHeal(AActor* a_AbilityUser)
{
	m_VFXComp->SetWorldLocation(a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f));

	if (!IsValid(m_VFXComp)) EndSelfHeal(a_AbilityUser);
}

void USelfHealAbilityAction::EndSelfHeal(AActor* a_AbilityUser)
{
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_MoveTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_StartTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Damage Circle Ability Action Ended"));
}
