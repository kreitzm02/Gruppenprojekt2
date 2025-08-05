// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Golem.h"
#include "Components/ArrowComponent.h"
#include "Enemy_GolemShockwave.h"
#include "Kismet/GameplayStatics.h"

AEnemy_Golem::AEnemy_Golem()
{
	m_shockwaveSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ShockwaveSpawnPoint"));
	m_shockwaveSpawnPoint->SetupAttachment(m_weaponMesh);
}

void AEnemy_Golem::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	m_attackDuration = m_attackAnimation->GetPlayLength();
}


void AEnemy_Golem::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_Golem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_smashReady && m_passedCooldownTime <= m_abilityCooldown)
	{
		m_passedCooldownTime += DeltaTime;

		if (m_passedCooldownTime >= m_abilityCooldown)
		{
			m_smashReady = true;
			m_passedCooldownTime = 0.0f;
		}
	}
}

void AEnemy_Golem::PlaySmashAttackSound(bool a_shouldLoop, float a_startPoint, float a_soundDuration)
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

void AEnemy_Golem::CreateShockwave()
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_shockwaveSpawnPoint->GetComponentLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_GolemShockwave* shockwave = GetWorld()->SpawnActorDeferred<AEnemy_GolemShockwave>(m_shockwaveBP, transform);
	shockwave->SetOwnerEnemy(this);
	shockwave->SetExpansionSpeed(m_shockwaveExpansionSpeed);
	shockwave->SetMaxRadius(m_shockwaveMaxRadius);
	shockwave->SetStartRadius(m_shockwaveStartRadius);
	UGameplayStatics::FinishSpawningActor(shockwave, transform);
	UE_LOG(LogTemp, Warning, TEXT("Shockwave fired"))
}
