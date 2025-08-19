// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/TornadoAbilityAction.h"
#include "NiagaraFunctionLibrary.h"
#include <Player/PlayerCharacter.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Game_GameInstance.h>
#include "Engine/StaticMeshActor.h"
#include "Engine/OverlapResult.h"
#include <EnemyCharacter.h>

void UTornadoAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
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

void UTornadoAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);
	if (!m_TornadoVFX || !a_AbilityUser) return;

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UTornadoAbilityAction::PlayTornado, a_AbilityUser), m_Delay, false);

	FTimerHandle fallBackDestroyHandle;
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(fallBackDestroyHandle, FTimerDelegate::CreateUObject(this, &UTornadoAbilityAction::EndAllTornados), m_Duration, false);
}

void UTornadoAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{
}

void UTornadoAbilityAction::PlayTornado(AActor* a_AbilityUser)
{
	// random spawn pos
	float angle = FMath::RandRange(0.0f, 2.0f * PI);
	float dist = FMath::RandRange(0.0f, m_SpawnRadius);
	FVector spawnPos = a_AbilityUser->GetActorLocation() + FVector(FMath::Cos(angle), FMath::Sin(angle), 0.0f) * dist;
	spawnPos.Z += 50.0f;

	// create tornado instance
	TSharedPtr<FTornadoInstance> inst = MakeShared<FTornadoInstance>();
	m_ActiveTornados.Add(inst);
	inst->m_CurrPosition = spawnPos;
	float dirAngle = FMath::RandRange(0.0f, 2.0f * PI);
	inst->m_Direction = FVector(FMath::Cos(dirAngle), FMath::Sin(dirAngle), 0.0f).GetSafeNormal();

	// niagara
	inst->m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_TornadoVFX, spawnPos, FRotator::ZeroRotator, FVector(1.0f), true, true, ENCPoolMethod::None, true);

	//sound
	inst->m_SoundComp = UGameplayStatics::SpawnSoundAttached(Cast<USoundBase>(m_SoundEffect), static_cast<USceneComponent*>(inst->m_VFXComp), NAME_None, FVector::ZeroVector, 
		EAttachLocation::KeepRelativeOffset, true, Cast<UGame_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetSFXVolume(), 1.0f, 0.0f, nullptr, nullptr, false);

	inst->m_SoundComp->bOverrideAttenuation = true;
	auto& A = inst->m_SoundComp->AttenuationOverrides;
	A.bAttenuate = true;
	A.AttenuationShape = EAttenuationShape::Sphere;
	A.AttenuationShapeExtents = FVector(300.f, 0.f, 0.f);
	A.FalloffDistance = m_AttractionRadius;

	// movement
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(inst->m_MoveHandle, FTimerDelegate::CreateUObject(this, &UTornadoAbilityAction::MoveTornadoTick, inst), 0.01f, true);

	// clear hit list
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(inst->m_ClearHitListHandle, FTimerDelegate::CreateUObject(this, &UTornadoAbilityAction::UpdateHitActors, inst), 1.0f, true);

	// end
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(inst->m_EndHandle, FTimerDelegate::CreateUObject(this, &UTornadoAbilityAction::EndTornado, inst), m_Duration, false);

}


void UTornadoAbilityAction::MoveTornadoTick(TSharedPtr<FTornadoInstance> a_Instance)
{
	FVector start = a_Instance->m_CurrPosition;
	FVector movePerTick = a_Instance->m_Direction * m_Speed * 0.01f;

	// wall bounce
	FHitResult hit;
	FCollisionObjectQueryParams objParams;
	objParams.AddObjectTypesToQuery(ECC_WorldStatic);
	objParams.AddObjectTypesToQuery(ECC_Visibility);

	bool wallHit = GetWorld()->SweepSingleByObjectType(hit, start, start + movePerTick, FQuat::Identity, objParams, FCollisionShape::MakeSphere(m_CollisionRadius));

	if (wallHit && Cast<AStaticMeshActor>(hit.GetActor()))
	{
		a_Instance->m_Direction = a_Instance->m_Direction.MirrorByPlane(FPlane(hit.Normal, 0)).GetSafeNormal();
		a_Instance->m_Direction.Z = 0.0f;
		a_Instance->m_CurrPosition = hit.ImpactPoint + hit.Normal * (m_CollisionRadius + 0.1f);
	}
	else
	{
		a_Instance->m_CurrPosition = start + movePerTick;
	}

	// change direction randomly
	const float maxDriftAngle = 10.0f;
	const float driftAngle = FMath::RandRange(-maxDriftAngle, maxDriftAngle);
	FVector2D dir2D(a_Instance->m_Direction.X, a_Instance->m_Direction.Y);
	dir2D = dir2D.GetRotated(driftAngle);
	a_Instance->m_Direction = FVector(dir2D.X, dir2D.Y, 0.0f).GetSafeNormal();

	// enemy damage
	TArray<FOverlapResult> overlaps;
	const FCollisionShape dmgShape = FCollisionShape::MakeSphere(m_CollisionRadius);
	bool anyOverlap = GetWorld()->OverlapMultiByChannel(overlaps, a_Instance->m_CurrPosition, FQuat::Identity, ECC_Pawn, dmgShape);

	if (anyOverlap)
	{
		for (const FOverlapResult& res : overlaps)
		{
			AActor* hitActor = res.GetActor();
			if (!hitActor || a_Instance->m_AlreadyHitActors.Contains(hitActor)) continue;

			a_Instance->m_AlreadyHitActors.Add(hitActor);

			UGame_GameInstance* gi = Cast<UGame_GameInstance>(GetWorld()->GetGameInstance());
			float dmg = m_Damage * gi->m_playerSave->GetPlayerDmgMultiplier() + gi->m_AdditionalDamage;
			UGameplayStatics::ApplyDamage(hitActor, dmg, nullptr, nullptr, nullptr);
		}
	}

	// enemy attraction

	//TArray<FOverlapResult> pullOverlaps;
	//FCollisionShape pullShape = FCollisionShape::MakeSphere(m_AttractionRadius);
	//
	//bool anyPulls = GetWorld()->OverlapMultiByChannel(pullOverlaps, a_Instance->m_CurrPosition, FQuat::Identity, ECC_Pawn, pullShape);
	//if (anyPulls)
	//{
	//	for (const FOverlapResult& pullRes : pullOverlaps)
	//	{
	//		AActor* actor = pullRes.GetActor();
	//		if (!actor || actor == nullptr) continue;
	//
	//		if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(actor))
	//		{
	//			if (UPrimitiveComponent* root = Cast<UPrimitiveComponent>(enemy->GetRootComponent()))
	//			{
	//				FVector toCenter = (a_Instance->m_CurrPosition - enemy->GetActorLocation());
	//				float dist = toCenter.Size();
	//				toCenter.Normalize();
	//
	//				float speed = m_AttractionStrength * (1.0f - FMath::Clamp(dist / m_AttractionRadius, 0.0f, 1.0f));
	//				enemy->SetActorLocation(enemy->GetActorLocation() + toCenter * speed * 0.01f);
	//			}
	//		}
	//	}
	//}


	TArray<FOverlapResult> pullOverlaps;
	const FCollisionShape pullShape = FCollisionShape::MakeSphere(m_AttractionRadius);

	if (GetWorld()->OverlapMultiByChannel(pullOverlaps, a_Instance->m_CurrPosition, FQuat::Identity, ECC_Pawn, pullShape))
	{
		float dt = FMath::Max(0.0f, GetWorld()->GetTimerManager().GetTimerRate(a_Instance->m_MoveHandle));

		for (const FOverlapResult& pullRes : pullOverlaps)
		{
			AActor* actor = pullRes.GetActor();
			if (!actor) continue;

			AEnemyCharacter* enemy = Cast<AEnemyCharacter>(actor);
			if (!enemy) continue;

			
			FHitResult losHit;
			FCollisionQueryParams qp(SCENE_QUERY_STAT(TornadoPullLOS), false, enemy);
			bool blocked = GetWorld()->LineTraceSingleByChannel(
				losHit,
				enemy->GetActorLocation(),
				a_Instance->m_CurrPosition,
				ECC_Visibility,
				qp
			);

			if (blocked)
			{
				continue;
			}

			FVector toCenter = a_Instance->m_CurrPosition - enemy->GetActorLocation();
			float dist = toCenter.Size();
			if (dist <= KINDA_SMALL_NUMBER) continue;

			toCenter /= dist; 
			float speed = m_AttractionStrength * (1.0f - FMath::Clamp(dist / m_AttractionRadius, 0.0f, 1.0f));
			FVector delta = toCenter * speed * dt;


			FHitResult sweepHit;
			enemy->SetActorLocation(enemy->GetActorLocation() + delta, true, &sweepHit, ETeleportType::None);
			if (sweepHit.IsValidBlockingHit())
			{
				FVector slide = FVector::VectorPlaneProject(delta, sweepHit.Normal);
				enemy->SetActorLocation(enemy->GetActorLocation() + slide * (1.f - sweepHit.Time), true);
			}
		}
	}



	// vfx
	if (a_Instance->m_VFXComp)
	{
		a_Instance->m_VFXComp->SetWorldLocation(a_Instance->m_CurrPosition);
		a_Instance->m_VFXComp->SetWorldRotation(a_Instance->m_Direction.Rotation());
	}
	else
	{
		EndTornado(a_Instance);
	}

}

void UTornadoAbilityAction::UpdateHitActors(TSharedPtr<FTornadoInstance> a_Instance)
{
	a_Instance->m_AlreadyHitActors.Empty();
}

void UTornadoAbilityAction::EndTornado(TSharedPtr<FTornadoInstance> a_Instance)
{
	if (a_Instance->m_SoundComp)
	{
		a_Instance->m_SoundComp->bAutoDestroy = true;
		a_Instance->m_SoundComp->FadeOut(0.5f, 0.f);
		a_Instance->m_SoundComp = nullptr;
	}

	if (a_Instance->m_VFXComp)
	{
		a_Instance->m_VFXComp->DeactivateImmediate();
		a_Instance->m_VFXComp->SetVisibility(false, true);
		a_Instance->m_VFXComp->SetAutoDestroy(true);
		a_Instance->m_VFXComp->DestroyComponent();
		a_Instance->m_VFXComp = nullptr;
	}
	m_ActiveTornados.Remove(a_Instance);
	GetWorld()->GetTimerManager().ClearTimer(a_Instance->m_MoveHandle);
	GetWorld()->GetTimerManager().ClearTimer(a_Instance->m_ClearHitListHandle);
}

void UTornadoAbilityAction::EndAllTornados()
{
	for (TSharedPtr<FTornadoInstance> inst : m_ActiveTornados)
	{
		EndTornado(inst);
	}
}
