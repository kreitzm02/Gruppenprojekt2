// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActions/TornadoAbilityAction.h"
#include "NiagaraFunctionLibrary.h"
#include <Player/PlayerCharacter.h>
#include <Kismet/KismetMathLibrary.h>

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
	inst->m_CurrPosition = spawnPos;
	float dirAngle = FMath::RandRange(0.0f, 2.0f * PI);
	inst->m_Direction = FVector(FMath::Cos(dirAngle), FMath::Sin(dirAngle), 0.0f).GetSafeNormal();

	// niagara
	inst->m_VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(a_AbilityUser->GetWorld(), m_TornadoVFX, spawnPos, FRotator::ZeroRotator, FVector(1.0f), true, true, ENCPoolMethod::None, true);

	// movement
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(inst->m_MoveHandle, FTimerDelegate::CreateUObject(this, &UTornadoAbilityAction::MoveTornadoTick, inst), 0.01f, true);

	// end
	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(inst->m_EndHandle, FTimerDelegate::CreateUObject(this, &UTornadoAbilityAction::EndTornado, inst), m_Duration, false);

}


void UTornadoAbilityAction::MoveTornadoTick(TSharedPtr<FTornadoInstance> a_Instance)
{
	FVector start = a_Instance->m_CurrPosition;
	FVector movePerTick = a_Instance->m_Direction * m_Speed * 0.01f;
	FHitResult hit;

	bool wallHit = GetWorld()->SweepSingleByChannel(hit, start, start + movePerTick, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(m_CollisionRadius));

	if (wallHit)
	{
		a_Instance->m_Direction = a_Instance->m_Direction.MirrorByVector(hit.Normal).GetSafeNormal();
		a_Instance->m_CurrPosition = hit.ImpactPoint + a_Instance->m_Direction * m_CollisionRadius;
	}
	else a_Instance->m_CurrPosition = start + movePerTick;

	FVector randDir = UKismetMathLibrary::RandomUnitVector();
	randDir.Z = 0.0f;
	randDir.Normalize();
	a_Instance->m_Direction = (a_Instance->m_Direction + randDir * m_Randomness * 0.01f).GetSafeNormal();

	if (a_Instance->m_VFXComp)
	{
		a_Instance->m_VFXComp->SetWorldLocation(a_Instance->m_CurrPosition);
		a_Instance->m_VFXComp->SetWorldRotation(a_Instance->m_Direction.Rotation());
	}
}

void UTornadoAbilityAction::EndTornado(TSharedPtr<FTornadoInstance> a_Instance)
{
	if (a_Instance->m_VFXComp)
	{
		a_Instance->m_VFXComp->DestroyComponent();
		a_Instance->m_VFXComp = nullptr;
	}

	GetWorld()->GetTimerManager().ClearTimer(a_Instance->m_MoveHandle);
}
