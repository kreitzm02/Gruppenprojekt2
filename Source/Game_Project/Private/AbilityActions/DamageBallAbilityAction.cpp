// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/DamageBallAbilityAction.h"
#include "Engine/OverlapResult.h"
#include <Player/PlayerCharacter.h>
#include <EnemyCharacter.h>
#include <Kismet/GameplayStatics.h>

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

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageBallAbilityAction::PlayDamageBall, a_AbilityUser), m_Delay, false);
}

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

void UDamageBallAbilityAction::PlayDamageBall(AActor* a_AbilityUser)
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		player->m_AnimInstance->Montage_Play(m_AttackMontage);
		// sound
		UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included damage ball ability action!"));

		const FRotator playerRotation = a_AbilityUser->GetActorRotation();
		const FVector forward = playerRotation.RotateVector(FVector::ForwardVector);
		const FVector offsetRotated = FRotator(0.0f, m_BallInitialRotation, 0.0f).RotateVector(forward);
		const FVector startPosition = a_AbilityUser->GetActorLocation() + offsetRotated * m_OffsetFromSpawn;

		TSharedRef<FDamageBallInstance> ballInstance = MakeShared<FDamageBallInstance>();
		ballInstance->m_Direction = offsetRotated.GetSafeNormal();
		ballInstance->m_CurrPosition = startPosition;

		FTimerHandle endTimerHandle;
		FTimerHandle moveTickHandle;

		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(moveTickHandle, FTimerDelegate::CreateLambda([=, this, &moveTickHandle, &endTimerHandle]()
		{
			// move
			FVector nextPos;

			if (m_Speed == 0.0f)
			{
				static float currentAngle = 0.0f;
				currentAngle += m_CircularRotationSpeed * 0.01f;
				if (currentAngle >= 360.0f) currentAngle -= 360.0f;

				const float radians = FMath::DegreesToRadians(currentAngle);
				const FVector circleOffset = FVector(FMath::Cos(radians), FMath::Sin(radians), 0.0f) * m_CircularMotionCircleRadius;

				FVector center = a_AbilityUser->GetActorLocation();
				nextPos = center + circleOffset;
			}
			else
			{
				const float step = m_Speed * 0.01f;
				nextPos = ballInstance->m_CurrPosition + ballInstance->m_Direction * step;
			}

			// wall bounce
			FHitResult hit;
			bool wallHit = a_AbilityUser->GetWorld()->LineTraceSingleByChannel(hit, ballInstance->m_CurrPosition, nextPos, ECC_Visibility);

			if (wallHit)
			{
				AActor* hitActor = hit.GetActor();
				if (ballInstance->m_HitCount >= m_CollisionsBeforeDestruction)
				{
					a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(moveTickHandle);
					return;
				}
				else if (m_BouncesOfWalls && hitActor && hitActor->IsRootComponentStatic() && ballInstance->m_HitCount <= m_CollisionsBeforeDestruction && !hitActor->IsA<AEnemyCharacter>())
				{
					const FVector Normal = hit.Normal;
					ballInstance->m_Direction = ballInstance->m_Direction.MirrorByPlane(FPlane(Normal, 0)).GetSafeNormal();
					ballInstance->m_Direction.Z = 0.0f;
					ballInstance->m_CurrPosition = hit.Location + ballInstance->m_Direction * 2.0f;
					ballInstance->m_HitCount++;
					return;
				}
			}

			// enemy hit
			TArray<FOverlapResult> overlaps;
			FCollisionShape shape = FCollisionShape::MakeSphere(m_BallSize);
			bool overlap = a_AbilityUser->GetWorld()->OverlapMultiByChannel(overlaps, nextPos, FQuat::Identity, ECC_Pawn, shape);

			if (overlap)
			{
				for (const FOverlapResult& result : overlaps)
				{
					AActor* hitActor = result.GetActor();
					if (hitActor && hitActor != a_AbilityUser && !ballInstance->m_AlreadyHitActors.Contains(hitActor))
					{
						ballInstance->m_AlreadyHitActors.Add(hitActor);
						ballInstance->m_HitCount++;
						UGameplayStatics::ApplyDamage(hitActor, m_Damage, nullptr, a_AbilityUser, nullptr);

						if (ballInstance->m_HitCount >= m_CollisionsBeforeDestruction)
						{
							a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(moveTickHandle);
							a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(endTimerHandle);
							return;
						}
					}
				}
			}

			// debug 
			DrawDebugSphere(a_AbilityUser->GetWorld(), nextPos, m_BallSize, 16, m_Color, false, 0.05f);

			// move
			ballInstance->m_CurrPosition = nextPos;
		}), 0.01f, true);

		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(endTimerHandle, FTimerDelegate::CreateLambda([=, this, &moveTickHandle, &endTimerHandle]()
		{
			a_AbilityUser->GetWorld()->GetTimerManager().ClearTimer(moveTickHandle);
			UE_LOG(LogTemp, Warning, TEXT("Damage Ball Auto-ended after duration"));
		}), m_MaxDuration, false);
	}
}
