// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/BaseAbilityAction.h"

void UBaseAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
}

void UBaseAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
}

void UBaseAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{
}

FName UBaseAbilityAction::GetWeaponMeshName(EAAWeaponNames a_WeaponMeshName) const
{
	switch (a_WeaponMeshName)
	{
	case EAAWeaponNames::MESH_1H_AXE:
		return FName("1H_Axe");
		break;
	case EAAWeaponNames::MESH_1H_CROSSBOW:
		return FName("1H_Crossbow");
		break;
	case EAAWeaponNames::MESH_1H_DAGGER:
		return FName("1H_Dagger");
		break;
	case EAAWeaponNames::MESH_1H_SPELLBOOK:
		return FName("1H_Scythe");
		break;
	case EAAWeaponNames::MESH_1H_SWORD:
		return FName("1H_Sword");
		break;
	case EAAWeaponNames::MESH_1H_WAND:
		return FName("1H_Wand");
		break;
	case EAAWeaponNames::MESH_DRUIDSTAFF:
		return FName("2H_Mace");
		break;
	case EAAWeaponNames::MESH_2H_STAFF:
		return FName("2H_Staff");
		break;
	default:
		return FName("1H_Axe");
		break;
	}
}
