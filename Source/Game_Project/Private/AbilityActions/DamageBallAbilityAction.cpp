// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/DamageBallAbilityAction.h"
#include "Engine/OverlapResult.h"
#include <Player/PlayerCharacter.h>
#include "NiagaraFunctionLibrary.h"
#include <EnemyCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <Game_GameInstance.h>

void UDamageBallAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
{
	Super::PrepareAbilityAction(a_AbilityUser);
	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		forceDestroyActive = false;

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

void UDamageBallAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (!m_VFX|| !a_AbilityUser) return;

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageBallAbilityAction::PlayDamageBall, a_AbilityUser), m_Delay, false);
}

void UDamageBallAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{
	forceDestroyActive = true;
}

void UDamageBallAbilityAction::MoveDamageBallTick(TSharedPtr<FDamageBallInstance> a_BallInstance, AActor* a_AbilityUser)
{
	FVector nextPos;
	FRotator rot;

	if (m_Speed == 0.0f)
	{
		a_BallInstance->m_CircleAngle += m_CircularRotationSpeed * 0.01f;
		if (a_BallInstance->m_CircleAngle >= 360.0f) a_BallInstance->m_CircleAngle -= 360.0f;
		const float radians = FMath::DegreesToRadians(a_BallInstance->m_CircleAngle);
		const FVector circleOffset = FVector(FMath::Cos(radians), FMath::Sin(radians), 0.0f) * m_CircularMotionCircleRadius;
		FVector center = a_AbilityUser->GetActorLocation();
		nextPos = center + circleOffset;
		const FVector radialDir = circleOffset.GetSafeNormal();
		const FVector tangentDir = FVector::CrossProduct(FVector::UpVector, radialDir).GetSafeNormal();
		a_BallInstance->m_Direction = tangentDir;
		rot = tangentDir.Rotation();
		rot.Pitch -= 90.0f;
	}
	else
	{
		const float step = m_Speed * 0.01f;
		nextPos = a_BallInstance->m_CurrPosition + a_BallInstance->m_Direction * step;
		rot = a_BallInstance->m_Direction.Rotation();
		rot.Pitch -= 90.0f;
	}

	// wall bounce
	FHitResult hit;
	bool wallHit = a_AbilityUser->GetWorld()->LineTraceSingleByChannel(hit, a_BallInstance->m_CurrPosition, nextPos, ECC_Visibility);

	if (wallHit)
	{
		AActor* hitActor = hit.GetActor();
		if (a_BallInstance->m_HitCount >= m_CollisionsBeforeDestruction || forceDestroyActive)
		{
			if (a_BallInstance->m_VFXComp)
			{
				a_BallInstance->m_VFXComp->DestroyComponent();
				a_BallInstance->m_VFXComp = nullptr;
			}
			EndDamageBall(a_BallInstance);
			UE_LOG(LogTemp, Warning, TEXT("Damage Ball ended after max collision"));
			return;
		}
		else if (m_BouncesOfWalls && hitActor && hitActor->IsRootComponentStatic() && a_BallInstance->m_HitCount <= m_CollisionsBeforeDestruction && !hitActor->IsA<AEnemyCharacter>())
		{
			const FVector Normal = hit.Normal;
			a_BallInstance->m_Direction = a_BallInstance->m_Direction.MirrorByPlane(FPlane(Normal, 0)).GetSafeNormal();
			a_BallInstance->m_Direction.Z = 0.0f;
			a_BallInstance->m_CurrPosition = hit.Location + a_BallInstance->m_Direction * 2.0f;
			a_BallInstance->m_HitCount++;
			a_BallInstance->m_AlreadyHitActors.Empty();
			return;
		}
	}

	// enemy hit
	TArray<FOverlapResult> overlaps;
	FCollisionShape shape = FCollisionShape::MakeSphere(m_BallSize * 1.99f);
	bool overlap = a_AbilityUser->GetWorld()->OverlapMultiByChannel(overlaps, nextPos, FQuat::Identity, ECC_Pawn, shape);

	if (overlap)
	{
		for (const FOverlapResult& result : overlaps)
		{
			AActor* hitActor = result.GetActor();
			if (hitActor && hitActor != a_AbilityUser && !a_BallInstance->m_AlreadyHitActors.Contains(hitActor))
			{
				a_BallInstance->m_AlreadyHitActors.Add(hitActor);
				a_BallInstance->m_HitCount++;
				UGame_GameInstance* gameInstance = Cast<UGame_GameInstance>(a_AbilityUser->GetWorld()->GetGameInstance());
				float finalAttackDamage = m_Damage * gameInstance->m_playerSave->GetPlayerDmgMultiplier() + gameInstance->m_AdditionalDamage;
				UGameplayStatics::ApplyDamage(hitActor, finalAttackDamage, nullptr, a_AbilityUser, nullptr);
				if (AEnemyCharacter* hitEnemy = Cast<AEnemyCharacter>(hitActor))
				{
					hitEnemy->TakeKnockback(m_KnockbackStrenght, hitEnemy->GetActorLocation() - a_BallInstance->m_CurrPosition);
				}

				if (a_BallInstance->m_HitCount >= m_CollisionsBeforeDestruction)
				{
					EndDamageBall(a_BallInstance);
					return;
				}
			}
		}
	}

	// debug 
	//DrawDebugSphere(a_AbilityUser->GetWorld(), nextPos, m_BallSize, 16, m_Color, false, 0.05f);

	if (a_BallInstance->m_VFXComp)
	{
		a_BallInstance->m_VFXComp->SetWorldLocation(nextPos);
		a_BallInstance->m_VFXComp->SetWorldRotation(rot);

	}

	// move
	a_BallInstance->m_CurrPosition = nextPos;
}

void UDamageBallAbilityAction::EndDamageBall(TSharedPtr<FDamageBallInstance> a_BallInstance)
{
	if (a_BallInstance->m_VFXComp)
	{
		a_BallInstance->m_VFXComp->DestroyComponent();
		a_BallInstance->m_VFXComp = nullptr;
	}
	GetWorld()->GetTimerManager().ClearTimer(a_BallInstance->m_MoveHandle);
	UE_LOG(LogTemp, Warning, TEXT("Damage Ball Auto-ended after duration"));
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
		const FRotator spawnRotation = FRotator(offsetRotated.Rotation().Pitch - 90.0f, offsetRotated.Rotation().Yaw, offsetRotated.Rotation().Roll);

		TSharedPtr<FDamageBallInstance> inst = MakeShared<FDamageBallInstance>();
		inst->m_Direction = offsetRotated.GetSafeNormal();
		inst->m_CurrPosition = startPosition;
		inst->m_CircleAngle = m_BallInitialRotation;

		FTimerHandle endTimerHandle;
		FTimerHandle moveTickHandle;

		

		if (m_VFX && !inst->m_VFXComp)
		{
			const FVector scale = FVector(m_BallSize * 0.01f);
			inst->m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_VFX, startPosition, spawnRotation, scale, true, true, ENCPoolMethod::None, true);
		}

		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(inst->m_MoveHandle, FTimerDelegate::CreateUObject(this, &UDamageBallAbilityAction::MoveDamageBallTick, inst, a_AbilityUser), 0.01f, true);
		
		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(inst->m_EndHandle, FTimerDelegate::CreateUObject(this, &UDamageBallAbilityAction::EndDamageBall, inst), m_MaxDuration, false);
	}
}
