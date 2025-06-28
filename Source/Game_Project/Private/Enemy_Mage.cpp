// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Mage.h"

#include "Enemy_MageFireballProjectile.h"
#include "Components/ArrowComponent.h"
#include "Enemy_MageProjectile.h"
#include "Kismet/GameplayStatics.h"

AEnemy_Mage::AEnemy_Mage()
{
	m_projectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	m_projectileSpawnPoint->SetupAttachment(m_weaponMesh);
}


void AEnemy_Mage::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	m_attackDuration = m_attackAnimation->GetPlayLength();
}

void AEnemy_Mage::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_Mage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_abilityReady && m_passedCooldownTime <= m_abilityCooldown)
	{
		m_passedCooldownTime += DeltaTime;
	
		if (m_passedCooldownTime >= m_abilityCooldown)
		{
			m_abilityReady = true;
			m_passedCooldownTime = 0.0f;
		}
	}
}

void AEnemy_Mage::FireProjectile(AActor* a_target)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_MageProjectile* projectile = GetWorld()->SpawnActorDeferred<AEnemy_MageProjectile>(m_projectileBP, transform);
	projectile->SetOwnerEnemy(this);
	projectile->SetProjectileSpeed(m_projectileSpeed);
	projectile->SetProjectileLifeSpan(m_projectileLifetime);
	projectile->SetTarget(a_target);
	UGameplayStatics::FinishSpawningActor(projectile, transform);
}

void AEnemy_Mage::FireFireball(AActor* a_target)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_MageFireballProjectile* fireball = GetWorld()->SpawnActorDeferred<AEnemy_MageFireballProjectile>(m_fireballBP, transform);
	fireball->SetOwnerEnemy(this);
	fireball->SetProjectileSpeed(m_fireballSpeed);
	fireball->SetProjectileLifeSpan(m_fireballLifetime);
	fireball->SetTarget(a_target);
	UGameplayStatics::FinishSpawningActor(fireball, transform);
}
