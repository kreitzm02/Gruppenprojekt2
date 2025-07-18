// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaAbilityAction.h"
#include <NiagaraFunctionLibrary.h>
#include <Player/PlayerCharacter.h>

FName UStaminaAbilityAction::GetWeaponMeshName() const
{
	switch (m_WeaponMeshName)
	{
	case EStaminaWeaponNames::MESH_CUSTOM:
		return m_CustomWeaponMeshName;
		break;
	case EStaminaWeaponNames::MESH_1H_AXE:
		return FName("1H_Axe");
		break;
	case EStaminaWeaponNames::MESH_1H_CROSSBOW:
		return FName("1H_Crossbow");
		break;
	case EStaminaWeaponNames::MESH_1H_DAGGER:
		return FName("1H_Dagger");
		break;
	case EStaminaWeaponNames::MESH_1H_SCYTHE:
		return FName("1H_Scythe");
		break;
	case EStaminaWeaponNames::MESH_1H_SWORD:
		return FName("1H_Sword");
		break;
	case EStaminaWeaponNames::MESH_1H_WAND:
		return FName("1H_Wand");
		break;
	case EStaminaWeaponNames::MESH_2H_MACE:
		return FName("2H_Mace");
		break;
	case EStaminaWeaponNames::MESH_2H_STAFF:
		return FName("2H_Staff");
		break;
	default:
		return FName("1H_Axe");
		break;
	}
}

void UStaminaAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(GetWeaponMeshName());

		if (m_AnimName == EStaminaAnimationNames::ANIM_SPELLCAST_LONG)
			m_AttackMontage = player->m_LongSpellcastMontage;
		else if (m_AnimName == EStaminaAnimationNames::ANIM_SPELLCAST_RAISE)
			m_AttackMontage = player->m_RaiseSpellcastMontage;
		else if (m_AnimName == EStaminaAnimationNames::ANIM_SPELLCAST_SHOOT)
			m_AttackMontage = player->m_ShootSpellcastMontage;
	}
}

void UStaminaAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (!m_StaminaVFX || !a_AbilityUser) return;

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UStaminaAbilityAction::PlayStamina, a_AbilityUser), m_StartingDelay, false);
}

void UStaminaAbilityAction::PlayStamina(AActor* a_AbilityUser)
{
	// niagara
	m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_StaminaVFX, a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f),
		FRotator::ZeroRotator, FVector(1.0f), true, true, ENCPoolMethod::None, true);

	// heal player
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->ChangePlayerStamina(m_StaminaAmount);
	}

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_MoveTimerHandle, FTimerDelegate::CreateUObject(this, &UStaminaAbilityAction::MoveStamina, a_AbilityUser), 0.01f, true);

	UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included stamina ability action!"))
}

void UStaminaAbilityAction::MoveStamina(AActor* a_AbilityUser)
{
	m_VFXComp->SetWorldLocation(a_AbilityUser->GetActorLocation() - FVector(0.0f, 0.0f, 50.0f));

	if (!IsValid(m_VFXComp)) EndStamina(a_AbilityUser);
}

void UStaminaAbilityAction::EndStamina(AActor* a_AbilityUser)
{
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_MoveTimerHandle);
	a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(m_StartTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Stamina Ability Action Ended"));
}
