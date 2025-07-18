// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy_Rogue.h"
#include "Enemy_RogueArrow.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

ABossEnemy_Rogue::ABossEnemy_Rogue()
{
	m_projectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	m_projectileSpawnPoint->SetupAttachment(m_weaponMesh);

}

void ABossEnemy_Rogue::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_weaponMesh->SetRelativeScale3D(m_weaponScale);
}

void ABossEnemy_Rogue::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	if (m_attackAnimation)
	{
		m_attackDuration = m_attackAnimation->GetPlayLength();
	}
}

void ABossEnemy_Rogue::BeginPlay()
{
	Super::BeginPlay();

	m_maxMultiplierAtHPPercent = 1 - m_maxMultiplierAtHPPercent;

	if (m_maxMultiplierAtHPPercent <= 0.0f)
	{
		m_maxMultiplierAtHPPercent = 0.01f;
	}
}


void ABossEnemy_Rogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_arrowSprayReady && !m_arrowCircleReady && m_passedCooldownTime <= m_abilityCooldown)
	{
		m_passedCooldownTime += DeltaTime * m_currentDoStuffMultiplier;

		if (m_passedCooldownTime >= m_abilityCooldown)
		{
			switch (FMath::RandRange(0, 1))
			{
			case 0:
				m_arrowSprayReady = true;
				break;
			case 1:
				m_arrowCircleReady = true;
				break;
			default:
				break;
			}
			m_passedCooldownTime = 0.0f;
		}
	}
}

float ABossEnemy_Rogue::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ABossEnemy_Rogue::FireArrow(AActor* a_target)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_RogueArrow* arrow = GetWorld()->SpawnActorDeferred<AEnemy_RogueArrow>(m_arrowBP, transform);
	arrow->SetOwnerEnemy(this);
	arrow->SetProjectileSpeed(m_arrowSpeed);
	arrow->SetProjectileLifeSpan(m_arrowLifetime);
	arrow->SetTarget(a_target);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

void ABossEnemy_Rogue::FireArrow(const FVector &a_targetDir)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_RogueArrow* arrow = GetWorld()->SpawnActorDeferred<AEnemy_RogueArrow>(m_arrowBP, transform);
	arrow->SetOwnerEnemy(this);
	arrow->SetProjectileSpeed(m_arrowSpeed);
	arrow->SetProjectileLifeSpan(m_arrowLifetime);
	arrow->SetTargetDirection(a_targetDir);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}
