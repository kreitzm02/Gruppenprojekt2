// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy_Warrior.h"

void ABossEnemy_Warrior::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_weaponMesh->SetRelativeScale3D(m_weaponScale);
}

void ABossEnemy_Warrior::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (m_attackAnimation)
	{
		m_attackDuration = m_attackAnimation->GetPlayLength();
	}
}

void ABossEnemy_Warrior::BeginPlay()
{
	Super::BeginPlay();

	m_maxMultiplierAtHPPercent = 1 - m_maxMultiplierAtHPPercent;

	if (m_maxMultiplierAtHPPercent <= 0.0f)
	{
		m_maxMultiplierAtHPPercent = 0.01f;
	}
}


void ABossEnemy_Warrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_chargeReady && !m_spinReady && m_passedCooldownTime <= m_abilityCooldown)
	{
		m_passedCooldownTime += DeltaTime * m_currentDoStuffMultiplier;

		if (m_passedCooldownTime >= m_abilityCooldown)
		{
			switch (FMath::RandRange(0, 1))
			{
			case 0:
				m_chargeReady = true;
				break;
			case 1:
				m_spinReady = true;
				break;
			default:
				break;
			}
			m_passedCooldownTime = 0.0f;
		}
	}
}

float ABossEnemy_Warrior::TakeDamage(float a_damageAmount, FDamageEvent const& a_damageEvent, AController* a_eventInstigator, AActor* a_damageCauser)
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

void ABossEnemy_Warrior::PlayChargingSound(bool a_shouldLoop, float a_startPoint, float a_soundDuration)
{
	if (a_soundDuration == 0.0f)
	{
		a_soundDuration = m_chargingSound->Duration - a_startPoint;
	}
	m_basicAttackSound->bLooping = false;
	m_ownActionSoundComp->Sound = m_basicAttackSound;

	GetWorld()->GetTimerManager().SetTimer(m_ownSoundPlayTimer, [this, a_startPoint]()
		{
			PlayOwnSound(a_startPoint);
		}, a_soundDuration, a_shouldLoop);
}

void ABossEnemy_Warrior::PlaySpinAttackSound(bool a_shouldLoop, float a_startPoint, float a_soundDuration)
{
	if (a_soundDuration == 0.0f)
	{
		a_soundDuration = m_spinAttackSound->Duration - a_startPoint;
	}
	m_basicAttackSound->bLooping = false;
	m_ownActionSoundComp->Sound = m_basicAttackSound;

	GetWorld()->GetTimerManager().SetTimer(m_ownSoundPlayTimer, [this, a_startPoint]()
		{
			PlayOwnSound(a_startPoint);
		}, a_soundDuration, a_shouldLoop);
}
