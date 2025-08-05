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

void ABossEnemy_Golem::PlaySmashAttackSound(bool a_shouldLoop, float a_startPoint, float a_soundDuration)
{
	if (a_soundDuration == 0.0f)
	{
		a_soundDuration = m_smashAttackSound->Duration - a_startPoint;
	}
	m_basicAttackSound->bLooping = false;
	m_ownActionSoundComp->Sound = m_smashAttackSound;
	m_ownActionSoundComp->Play(a_startPoint);

	if (a_shouldLoop)
	GetWorld()->GetTimerManager().SetTimer(m_ownSoundPlayTimer, [this, a_startPoint]()
		{
			PlayOwnSound(a_startPoint);
		}, a_soundDuration, a_shouldLoop);
}

void ABossEnemy_Golem::PlayJumpSound(bool a_shouldLoop, float a_startPoint, float a_soundDuration)
{
	if (a_soundDuration == 0.0f)
	{
		a_soundDuration = m_jumpSound->Duration - a_startPoint;
	}
	m_basicAttackSound->bLooping = false;
	m_ownActionSoundComp->Sound = m_jumpSound;
	m_ownActionSoundComp->Play(a_startPoint);

	if (a_shouldLoop)
	GetWorld()->GetTimerManager().SetTimer(m_ownSoundPlayTimer, [this, a_startPoint]()
		{
			PlayOwnSound(a_startPoint);
		}, a_soundDuration, a_shouldLoop);
}

float ABossEnemy_Golem::TakeDamage(float a_damageAmount, FDamageEvent const& a_damageEvent, AController* a_eventInstigator, AActor* a_damageCauser)
{
	m_currentHealth = FMath::Clamp(m_currentHealth - a_damageAmount, 0.0f, m_maxHealth);

	m_currentDoStuffMultiplier = 1 + (1 - m_currentHealth / m_maxHealth) / m_maxMultiplierAtHPPercent * (m_doStuffMaxMultiplier - 1);
	m_currentDoStuffMultiplier = FMath::Clamp(m_currentDoStuffMultiplier, 1.0f, m_doStuffMaxMultiplier);


	UpdateHealthBar();

	m_receivingActionSoundComp->Sound = m_hitSound;
	m_receivingActionSoundComp->Play(0.0f);

	if (m_currentHealth <= 0.0f && !m_isDead)
	{
		GetWorld()->SpawnActor<ADungeonExitPortal>(m_dungeonExitBP, GetActorLocation(), FRotator::ZeroRotator);
		OnDeath();
	}

	return a_damageAmount;
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