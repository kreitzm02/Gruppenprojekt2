// Fill out your copyright notice in the Description page of Project Settings.
           
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "AbilityActions/DamageCircleAbilityAction.h"
#include <Player/PlayerCharacter.h>

void UDamageCircleAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->HideAllWeaponsExcept(GetWeaponMeshName());

		if (m_AnimName == EDamageCircleAnimationNames::ANIM_SPELLCAST_LONG)
			m_AttackMontage = player->m_LongSpellcastMontage;
		else if (m_AnimName == EDamageCircleAnimationNames::ANIM_SPELLCAST_RAISE)
			m_AttackMontage = player->m_RaiseSpellcastMontage;
		else if (m_AnimName == EDamageCircleAnimationNames::ANIM_SPELLCAST_SHOOT)
			m_AttackMontage = player->m_ShootSpellcastMontage;
	}
}

void UDamageCircleAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::PlayDamageCircle, a_AbilityUser), m_Delay, false);
}

FName UDamageCircleAbilityAction::GetWeaponMeshName() const
{
	switch (m_WeaponMeshName)
	{
	case EDamageCircleWeaponNames::MESH_CUSTOM:
		return m_CustomWeaponMeshName;
		break;
	case EDamageCircleWeaponNames::MESH_1H_AXE:
		return FName("1H_Axe");
		break;
	case EDamageCircleWeaponNames::MESH_1H_CROSSBOW:
		return FName("1H_Crossbow");
		break;
	case EDamageCircleWeaponNames::MESH_1H_DAGGER:
		return FName("1H_Dagger");
		break;
	case EDamageCircleWeaponNames::MESH_1H_SCYTHE:
		return FName("1H_Scythe");
		break;
	case EDamageCircleWeaponNames::MESH_1H_SWORD:
		return FName("1H_Sword");
		break;
	case EDamageCircleWeaponNames::MESH_1H_WAND:
		return FName("1H_Wand");
		break;
	case EDamageCircleWeaponNames::MESH_2H_MACE:
		return FName("2H_Mace");
		break;
	case EDamageCircleWeaponNames::MESH_2H_STAFF:
		return FName("2H_Staff");
		break;
	default:
		return FName("1H_Axe");
		break;
	}
}

void UDamageCircleAbilityAction::EndAbilityAction()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(m_DamageTickTimerHandle);
		World->GetTimerManager().ClearTimer(m_DebugDrawTimerHandle);
		World->GetTimerManager().ClearTimer(m_EndTimerHandle);
		World->GetTimerManager().ClearTimer(m_StartTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Damage Circle Ability Action Ended"));
	}
}

void UDamageCircleAbilityAction::DrawDebug(AActor* a_AbilityUser)
{
	FVector location = m_CircleFollowsUser ? a_AbilityUser->GetActorLocation() : m_StaticCircleLocation;
	DrawDebugSphere(a_AbilityUser->GetWorld(), location, m_CircleSize, 32, m_Color, false, 0.06f, 0, 1.0f);
}

void UDamageCircleAbilityAction::PerformDamageTick(AActor* a_AbilityUser)
{
	m_AlreadyHitActors.Empty();

	FVector location;
	if (m_CircleFollowsUser) location = a_AbilityUser->GetActorLocation();
	else location = m_StaticCircleLocation;

	TArray<FOverlapResult> overlapResults;

	FCollisionShape collisionShape = FCollisionShape::MakeSphere(m_CircleSize);

	bool hasOverlap = a_AbilityUser->GetWorld()->OverlapMultiByChannel(overlapResults, location, FQuat::Identity, ECollisionChannel::ECC_Pawn, collisionShape);

	if (hasOverlap)
	{
		for (int i = 0; i < overlapResults.Num(); i++)
		{
			AActor* hitActor = overlapResults[i].GetActor();
			if (hitActor && hitActor != a_AbilityUser)
			{
				if (m_AlreadyHitActors.Contains(hitActor)) continue;
				m_AlreadyHitActors.Add(hitActor);
				UGameplayStatics::ApplyDamage(hitActor, m_DamagePerHit, nullptr, a_AbilityUser, nullptr);
			}
		}
	}
}

void UDamageCircleAbilityAction::PlayDamageCircle(AActor* a_AbilityUser)
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->m_AnimInstance->Montage_Play(m_AttackMontage);
		// sound
		m_StaticCircleLocation = a_AbilityUser->GetActorLocation();

		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_DebugDrawTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::DrawDebug, a_AbilityUser), 0.05f, true);
		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_DamageTickTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::PerformDamageTick, a_AbilityUser), m_TimeBetweenHits, true);
		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_EndTimerHandle, this, &UDamageCircleAbilityAction::EndAbilityAction, m_Duration, false);

		UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included damage circle ability action!"))
	}
}
