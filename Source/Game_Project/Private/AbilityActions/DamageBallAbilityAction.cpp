// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/DamageBallAbilityAction.h"
#include "Engine/OverlapResult.h"
#include <Player/PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>

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
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(m_MoveTickHandle);
		World->GetTimerManager().ClearTimer(m_EndTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Damage Ball Ability Action Ended"));
	}
}

void UDamageBallAbilityAction::UpdateBallMovement(AActor* a_AbilityUser)
{
	const float step = m_Speed * 0.01f;
	const FVector nextPos = m_CurrPosition + m_Direction * step;

	FHitResult hit;
	const bool wallHit = a_AbilityUser->GetWorld()->LineTraceSingleByChannel(hit, m_CurrPosition, nextPos, ECC_Visibility);

	if (wallHit)
	{
		AActor* hitActor = hit.GetActor();
		if (m_BouncesOfWalls && hitActor->IsRootComponentStatic())
		{
			const FVector Normal = hit.Normal;
			m_Direction = m_Direction.MirrorByPlane(FPlane(hit.Normal, 0)).GetSafeNormal();
			m_CurrPosition = hit.Location + m_Direction * 2.0f;
			return;
		}
		else
		{
			EndAbilityAction();
			return;
		}
	}
	TArray<FOverlapResult> overlapResults;

	FCollisionShape collisionShape = FCollisionShape::MakeSphere(m_BallSize);

	bool hasOverlap = a_AbilityUser->GetWorld()->OverlapMultiByChannel(overlapResults, nextPos, FQuat::Identity, ECollisionChannel::ECC_Pawn, collisionShape);

	if (hasOverlap)
	{
		for (int i = 0; i < overlapResults.Num(); i++)
		{
			AActor* hitActor = overlapResults[i].GetActor();
			if (hitActor && hitActor != a_AbilityUser)
			{
				if (m_AlreadyHitActors.Contains(hitActor)) continue;
				m_AlreadyHitActors.Add(hitActor);
				m_HitAmount++;
				UGameplayStatics::ApplyDamage(hitActor, m_Damage, nullptr, a_AbilityUser, nullptr);

				if (m_HitAmount >= m_CollisionsBeforeDestruction)
				{
					EndAbilityAction();
					return;
				}
			}
		}
	}

	DrawDebugSphere(a_AbilityUser->GetWorld(), nextPos, m_BallSize, 16, m_Color, false, 0.05f);

	m_CurrPosition = nextPos;
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
		m_Direction = offsetRotated.GetSafeNormal();

		m_CurrPosition = a_AbilityUser->GetActorLocation() + offsetRotated * m_OffsetFromSpawn;
		m_HitAmount = 0;
		m_AlreadyHitActors.Empty();

		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_MoveTickHandle, FTimerDelegate::CreateUObject(this, &UDamageBallAbilityAction::UpdateBallMovement, a_AbilityUser), 0.01f, true);
		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_EndTimerHandle, this, &UDamageBallAbilityAction::EndAbilityAction, m_MaxDuration, false);
	}
}
