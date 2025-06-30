// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Necromancer.h"
#include "Enemy_NecroSummon.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


void AEnemy_Necromancer::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	m_attackDuration = m_attackAnimation->GetPlayLength();
}

void AEnemy_Necromancer::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_Necromancer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_summonReady && m_passedCooldownTime <= m_summonCooldown)
	{
		m_passedCooldownTime += DeltaTime;

		if (m_passedCooldownTime >= m_summonCooldown)
		{
			m_summonReady = true;
			m_passedCooldownTime = 0.0f;
		}
	}
}

void AEnemy_Necromancer::FireProjectile(FVector a_direction)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Necro Projectile"))
}


void AEnemy_Necromancer::Summon()
{
	for (; m_currentSummons < m_maxSummons; m_currentSummons++)
	{
		FVector location = FVector::ZeroVector;

		UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(this->GetWorld());
		if (!navSys) return;

		FVector origin = this->GetActorLocation();
		FNavLocation navLocation;

		if (navSys->GetRandomReachablePointInRadius(origin, m_summonSpawnRadius, navLocation))
		{
			location = navLocation.Location;
		}

		location.Z = this->GetActorLocation().Z + m_summonsHeightOffset;
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FVector scale(1.0f, 1.0f, 1.0f);

		FTransform spawnTransform(rotation, location, scale);

		AEnemy_NecroSummon* necroSummon = GetWorld()->SpawnActorDeferred<AEnemy_NecroSummon>(m_necroSummonBP, spawnTransform);
		necroSummon->GetCharacterMovement()->GravityScale = 0.0f;
		necroSummon->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		necroSummon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		necroSummon->SetHeightOffset(m_summonsHeightOffset);
		necroSummon->SetOwnerNecromancer(this);
		UGameplayStatics::FinishSpawningActor(necroSummon, spawnTransform);
	}
}

void AEnemy_Necromancer::SubstractSummon()
{
	m_currentSummons -= 1;
}
