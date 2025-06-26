// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/DamageBallAbilityAction.h"
#include <Player/PlayerCharacter.h>

FName UDamageBallAbilityAction::GetWeaponMeshName() const
{
	switch (m_WeaponMeshName)
	{
	case EDamageBallWeaponNames::MESH_CUSTOM:
		return m_CustomWeaponMeshName;
		break;
	case EDamageBallWeaponNames::MESH_1H_AXE:
		return FName("1H_Axe");
		break;
	case EDamageBallWeaponNames::MESH_1H_CROSSBOW:
		return FName("1H_Crossbow");
		break;
	case EDamageBallWeaponNames::MESH_1H_DAGGER:
		return FName("1H_Dagger");
		break;
	case EDamageBallWeaponNames::MESH_1H_SCYTHE:
		return FName("1H_Scythe");
		break;
	case EDamageBallWeaponNames::MESH_1H_SWORD:
		return FName("1H_Sword");
		break;
	case EDamageBallWeaponNames::MESH_1H_WAND:
		return FName("1H_Wand");
		break;
	case EDamageBallWeaponNames::MESH_2H_MACE:
		return FName("2H_Mace");
		break;
	case EDamageBallWeaponNames::MESH_2H_STAFF:
		return FName("2H_Staff");
		break;
	default:
		return FName("1H_Axe");
		break;
	}
}

void UDamageBallAbilityAction::EndAbilityAction()
{
}

void UDamageBallAbilityAction::DrawDebug(AActor* a_AbilityUser)
{
}

void UDamageBallAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(GetWeaponMeshName());

		if (m_AnimName == EDamageBallAnimationNames::ANIM_SPELLCAST_LONG)
			m_AttackMontage = player->m_LongSpellcastMontage;
		else if (m_AnimName == EDamageBallAnimationNames::ANIM_SPELLCAST_RAISE)
			m_AttackMontage = player->m_RaiseSpellcastMontage;
		else if (m_AnimName == EDamageBallAnimationNames::ANIM_SPELLCAST_SHOOT)
			m_AttackMontage = player->m_ShootSpellcastMontage;
	}
}

void UDamageBallAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);

	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->m_AnimInstance->Montage_Play(m_AttackMontage);
		// sound
		UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included damage ball ability action!"));

		const FRotator playerRotation = a_AbilityUser->GetActorRotation();
		const FVector forward = playerRotation.RotateVector(FVector::ForwardVector);
		const FVector offsetRotated = FRotator(0.0f, m_BallInitialRotation, 0.0f).RotateVector(forward); 
	}
}
