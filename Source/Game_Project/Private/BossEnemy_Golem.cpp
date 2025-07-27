// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy_Golem.h"

#include "Enemy_GolemBoulder.h"
#include "Components/ArrowComponent.h"
#include "Enemy_GolemShockwave.h"
#include "Kismet/GameplayStatics.h"

ABossEnemy_Golem::ABossEnemy_Golem()
{
	m_shockwaveWeaponSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ShockwaveSmashSpawnPoint"));
	m_shockwaveWeaponSpawnPoint->SetupAttachment(m_weaponMesh);

	m_shockwaveJumpSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ShockwaveJumpSpawnPoint"));
	m_shockwaveJumpSpawnPoint->SetupAttachment(m_skeletalMesh);
}

void ABossEnemy_Golem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_weaponMesh->SetRelativeScale3D(m_weaponScale);
}

void ABossEnemy_Golem::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (m_attackAnimation)
	{
		m_attackDuration = m_attackAnimation->GetPlayLength();
	}
}


void ABossEnemy_Golem::BeginPlay()
{
	Super::BeginPlay();

	m_maxMultiplierAtHPPercent = 1 - m_maxMultiplierAtHPPercent;

	if (m_maxMultiplierAtHPPercent <= 0.0f)
	{
		m_maxMultiplierAtHPPercent = 0.01f;
	}
}


void ABossEnemy_Golem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_jumpReady && !m_smashReady && m_passedCooldownTime <= m_abilityCooldown)
	{
		m_passedCooldownTime += DeltaTime * m_currentDoStuffMultiplier;

		if (m_passedCooldownTime >= m_abilityCooldown)
		{
			switch (FMath::RandRange(0, 1))
			{
			case 0:
				m_jumpReady = true;
				break;
			case 1:
				m_smashReady = true;
				break;
			default:
				break;
			}
			m_passedCooldownTime = 0.0f;
		}
	}
}

float ABossEnemy_Golem::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

	if (m_currentHealth <= 0.0f && !m_isDead)
	{
		GetWorld()->SpawnActor<ADungeonExitPortal>(m_dungeonExitBP, GetActorLocation(), FRotator::ZeroRotator);
		OnDeath();
	}

	return DamageAmount;
}

void ABossEnemy_Golem::CreateSmashShockwaveWithBoulder(AActor* a_boulderTarget)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_shockwaveWeaponSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_GolemShockwave* shockwave = GetWorld()->SpawnActorDeferred<AEnemy_GolemShockwave>(m_shockwaveBP, transform);
	shockwave->SetOwnerEnemy(this);
	shockwave->SetExpansionSpeed(m_shockwaveExpansionSpeed);
	shockwave->SetMaxRadius(m_shockwaveMaxRadius);
	shockwave->SetStartRadius(m_shockwaveStartRadius);
	UGameplayStatics::FinishSpawningActor(shockwave, transform);

	FVector boulderPos = a_boulderTarget->GetActorLocation();
	boulderPos.Z += m_boulderHeightOffset;

	transform = FTransform(FRotator::ZeroRotator, boulderPos,FVector::OneVector);
	AEnemy_GolemBoulder* boulder = GetWorld()->SpawnActorDeferred<AEnemy_GolemBoulder>(m_boulderBP, transform);
	boulder->SetOwner(this);
	UGameplayStatics::FinishSpawningActor(boulder, transform);

	UE_LOG(LogTemp, Warning, TEXT("Shockwave fired"))
}

void ABossEnemy_Golem::CreateJumpShockwave()
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_shockwaveJumpSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_GolemShockwave* shockwave = GetWorld()->SpawnActorDeferred<AEnemy_GolemShockwave>(m_shockwaveBP, transform);
	shockwave->SetOwnerEnemy(this);
	shockwave->SetExpansionSpeed(m_shockwaveExpansionSpeed);
	shockwave->SetMaxRadius(m_shockwaveMaxRadius);
	shockwave->SetStartRadius(m_shockwaveStartRadius);
	UGameplayStatics::FinishSpawningActor(shockwave, transform);
	UE_LOG(LogTemp, Warning, TEXT("Shockwave fired"))
}