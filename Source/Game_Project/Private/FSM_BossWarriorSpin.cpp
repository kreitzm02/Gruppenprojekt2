// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorSpin.h"

#include "StaticMeshAttributes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UFSM_BossWarriorSpin::Initialize()
{
	Super::Initialize();
	ABossEnemy_Warrior* enemy = Cast<ABossEnemy_Warrior>(m_ownerCharacter);
	m_chargeAnimation = enemy->GetSpinAnimation();
	m_spinSpeed = enemy->GetSpinSpeed();
	m_detectionRange = enemy->GetPlayerChaseRadius();
	m_owner = enemy;
}

void UFSM_BossWarriorSpin::OnEnter()
{
	Super::OnEnter();

	m_ownerSkeletalMesh->PlayAnimation(m_chargeAnimation, true);

	if (m_ownerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Charge has no Owner Pawn!"))
	}
	else
	{
		TArray<FOverlapResult> overlaps;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(m_ownerCharacter);

		FCollisionObjectQueryParams objectQueryParams;
		objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

		bool hasOverlap = m_ownerCharacter->GetWorld()->OverlapMultiByObjectType(
			overlaps,
			m_ownerCharacter->GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(m_detectionRange),
			queryParams
		);
		for (FOverlapResult& overlap : overlaps)
		{
			AActor* actor = overlap.GetActor();
			if (actor && actor->IsA(ACharacter::StaticClass()))
			{
				m_player = Cast<ACharacter>(actor);
			}
		}

		m_owner->GetCharacterMovement()->MaxWalkSpeed = m_spinSpeed * m_owner->GetMultiplier();
	}

	m_spinDirectionNormal = (m_player->GetActorLocation() - m_ownerCharacter->GetActorLocation()).GetSafeNormal();

}

void UFSM_BossWarriorSpin::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);

	m_ownerCharacter->AddMovementInput(m_spinDirectionNormal);

	FHitResult hitResult;
	FVector start = m_ownerCharacter->GetActorLocation();
	FVector end = m_ownerCharacter->GetActorLocation() + (m_spinDirectionNormal * m_changeDirBeforeWallMultiplier);

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(m_ownerCharacter);
	traceParams.AddIgnoredActor(m_player);

	bool hit = m_ownerCharacter->GetWorld()->LineTraceSingleByChannel(
		hitResult,
		start,
		end,
		ECC_Visibility,
		traceParams
	);

	if (m_enableDebug)
	{
		DrawDebugLine(
			m_ownerCharacter->GetWorld(),
			start,
			end,
			hit ? FColor::Red : FColor::Green,
			false,
			0.1f,
			0,
			1.0f
		);
	}

	if (hit)
	{
		UE_LOG(LogTemp, Warning, TEXT("boss collided with wall"));
		UE_LOG(LogTemp, Warning, TEXT("normal vector x:%f y:%f z:%f"), m_spinDirectionNormal.X, m_spinDirectionNormal.Y, m_spinDirectionNormal.Z);
		m_spinDirectionNormal = FVector::VectorPlaneProject(m_spinDirectionNormal, hitResult.Normal).GetSafeNormal();
		//m_spinDirectionNormal.MirrorByPlane(FPlane(hitResult.ImpactPoint, hitResult.Normal));
		UE_LOG(LogTemp, Warning, TEXT("normal vector x:%f y:%f z:%f"), m_spinDirectionNormal.X, m_spinDirectionNormal.Y, m_spinDirectionNormal.Z);
	}
	
}

void UFSM_BossWarriorSpin::OnExit()
{
	Super::OnExit();

	m_owner->SetSpinReady(false);
}