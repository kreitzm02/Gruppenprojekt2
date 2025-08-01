// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityActions/DamageCircleAbilityAction.h"         
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "Engine/StaticMeshActor.h"
#include <Player/PlayerCharacter.h>
#include <Game_GameInstance.h>

void UDamageCircleAbilityAction::PrepareAbilityAction(AActor* a_AbilityUser)
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
	}
}

void UDamageCircleAbilityAction::PlayAbilityAction(AActor* a_AbilityUser)
{
	Super::PlayAbilityAction(a_AbilityUser);

	a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_StartTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::PlayDamageCircle, a_AbilityUser), m_Delay, false);
}

void UDamageCircleAbilityAction::EndAbilityAction(AActor* a_AbilityUser)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(m_DamageTickTimerHandle);
		World->GetTimerManager().ClearTimer(m_DebugDrawTimerHandle);
		World->GetTimerManager().ClearTimer(m_CircleMoveTimerHandle);
		World->GetTimerManager().ClearTimer(m_EndTimerHandle);
		World->GetTimerManager().ClearTimer(m_StartTimerHandle);
		m_DCMeshActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Damage Circle Ability Action Ended"));
	}
}

void UDamageCircleAbilityAction::DrawDebug(AActor* a_AbilityUser)
{
	FVector location = m_CircleFollowsUser ? a_AbilityUser->GetActorLocation() : m_StaticCircleLocation;
	DrawDebugSphere(a_AbilityUser->GetWorld(), location, m_CircleSize, 32, m_Color, false, 0.06f, 0, 1.0f);
}

void UDamageCircleAbilityAction::MoveCircle(AActor* a_AbilityUser)
{
	FVector location = m_CircleFollowsUser ? a_AbilityUser->GetActorLocation() : m_StaticCircleLocation;
	location.Z -= 80.0f;
	m_DCMeshActor->SetActorLocation(location);
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
				float finalAttackDamage = m_DamagePerHit * Cast<UGame_GameInstance>(a_AbilityUser->GetWorld()->GetGameInstance())->m_playerSave->GetPlayerDmgMultiplier();
				UGameplayStatics::ApplyDamage(hitActor, finalAttackDamage, nullptr, a_AbilityUser, nullptr);
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

		if (m_DCMesh)
		{
			FVector location = a_AbilityUser->GetActorLocation();
			location.Z -= 80.0f;
			m_DCMeshActor = a_AbilityUser->GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), location, FRotator::ZeroRotator);
			if (m_DCMeshActor)
			{
				UStaticMeshComponent* meshComp = m_DCMeshActor->GetStaticMeshComponent();
				m_DCMeshActor->SetMobility(EComponentMobility::Movable);
				meshComp->SetStaticMesh(m_DCMesh);
				m_DCMeshActor->SetActorEnableCollision(false);
				m_DCMeshActor->SetActorScale3D(m_DCMeshActor->GetActorScale3D() * FVector(m_CircleSize * 0.005, m_CircleSize * 0.005, 1));
				UMaterialInstanceDynamic* dynMat = meshComp->CreateAndSetMaterialInstanceDynamic(0);
				dynMat->SetVectorParameterValue(FName("color"), m_Color.ReinterpretAsLinear());
			}
		}

		//a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_DebugDrawTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::DrawDebug, a_AbilityUser), 0.05f, true);
		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_CircleMoveTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::MoveCircle, a_AbilityUser), 0.01f, true);
		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_DamageTickTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::PerformDamageTick, a_AbilityUser), m_TimeBetweenHits, true);
		a_AbilityUser->GetWorld()->GetTimerManager().SetTimer(m_EndTimerHandle, FTimerDelegate::CreateUObject(this, &UDamageCircleAbilityAction::EndAbilityAction, a_AbilityUser), m_Duration, false);

		UE_LOG(LogTemp, Warning, TEXT("Player just used an ability that included damage circle ability action!"))
	}
}
