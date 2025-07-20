// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy_Mage.h"

#include "Enemy_BurnGroundProjectile.h"
#include "Enemy_MageFireballProjectile.h"
#include "Enemy_MageProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABossEnemy_Mage::ABossEnemy_Mage()
{
	m_projectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	m_projectileSpawnPoint->SetupAttachment(m_weaponMesh);
}

void ABossEnemy_Mage::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_weaponMesh->SetRelativeScale3D(m_weaponScale);
}

void ABossEnemy_Mage::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (m_attackAnimation)
	{
		m_attackDuration = m_attackAnimation->GetPlayLength();
	}
}

void ABossEnemy_Mage::BeginPlay()
{
	Super::BeginPlay();

	m_maxMultiplierAtHPPercent = 1 - m_maxMultiplierAtHPPercent;

	if (m_maxMultiplierAtHPPercent <= 0.0f)
	{
		m_maxMultiplierAtHPPercent = 0.01f;
	}
}


void ABossEnemy_Mage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_burnGroundReady && !m_fireballReady && m_passedCooldownTime <= m_abilityCooldown)
	{
		m_passedCooldownTime += DeltaTime * m_currentDoStuffMultiplier;

		if (m_passedCooldownTime >= m_abilityCooldown)
		{
			switch (FMath::RandRange(1, 1))
			{
			case 0:
				m_burnGroundReady = true;
				break;
			case 1:
				m_fireballReady = true;
				break;
			default:
				break;
			}
			m_passedCooldownTime = 0.0f;
		}
	}
}

float ABossEnemy_Mage::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	m_currentHealth = FMath::Clamp(m_currentHealth - DamageAmount, 0.0f, m_maxHealth);

	m_currentDoStuffMultiplier = 1 + (1 - m_currentHealth / m_maxHealth) / m_maxMultiplierAtHPPercent * (m_doStuffMaxMultiplier - 1);
	m_currentDoStuffMultiplier = FMath::Clamp(m_currentDoStuffMultiplier, 1.0f, m_doStuffMaxMultiplier);

	if (DamageCauser)
	{
		FVector knockbackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		knockbackDirection.Z = 0;
		knockbackDirection.Normalize();
		HandleKnockback(knockbackDirection, 600.0f /*get knockback strengh from damage causer*/);
		//ACharacter* damagingUnit = Cast<ACharacter>(DamageCauser);
		//damagingUnit->GetKnockback();
	}

	UpdateHealthBar();

	if (m_currentHealth <= 0.0f)
	{
		OnDeath();
	}

	return DamageAmount;
}

void ABossEnemy_Mage::FireProjectile(AActor* a_target)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_MageProjectile* projectile = GetWorld()->SpawnActorDeferred<AEnemy_MageProjectile>(m_projectileBP, transform);
	projectile->SetOwnerEnemy(this);
	projectile->SetProjectileSpeed(m_projectileSpeed);
	projectile->SetProjectileLifeSpan(m_projectileLifetime);
	projectile->SetTarget(a_target);
	UGameplayStatics::FinishSpawningActor(projectile, transform);
}

void ABossEnemy_Mage::FireFireball(AActor* a_target)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f) * m_currentDoStuffMultiplier);

	AEnemy_MageFireballProjectile* fireball = GetWorld()->SpawnActorDeferred<AEnemy_MageFireballProjectile>(m_fireballBP, transform);
	fireball->SetOwnerEnemy(this);
	fireball->SetProjectileSpeed(m_fireballSpeed);
	fireball->SetProjectileLifeSpan(m_fireballLifetime);
	fireball->SetTarget(a_target);
	UGameplayStatics::FinishSpawningActor(fireball, transform);
}

void ABossEnemy_Mage::FireBurnGround(ACharacter* a_target)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	FVector targetPos = a_target->GetActorLocation();
	targetPos.Z -= a_target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	AEnemy_BurnGroundProjectile* burnGround = GetWorld()->SpawnActorDeferred<AEnemy_BurnGroundProjectile>(m_fireballBP, transform);
	burnGround->SetOwner(this);
	burnGround->SetTargetPos(targetPos);
	burnGround->SetFlightTime(m_burnGroundFlightTime);
	burnGround->SetLifetime(m_burnGroundLifetime * m_currentDoStuffMultiplier);
	UGameplayStatics::FinishSpawningActor(burnGround, transform);
}
