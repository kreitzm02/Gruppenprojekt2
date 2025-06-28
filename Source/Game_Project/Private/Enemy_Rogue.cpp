// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Rogue.h"
#include "Enemy_RogueArrow.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy_Rogue::AEnemy_Rogue()
{
	m_projectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	m_projectileSpawnPoint->SetupAttachment(m_weaponMesh);
}

void AEnemy_Rogue::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	m_attackDuration = m_attackAnimation->GetPlayLength();
}

void AEnemy_Rogue::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_Rogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_arrowSprayReady && m_passedCooldownTime <= m_arrowSprayCooldown)
	{
		m_passedCooldownTime += DeltaTime;

		if (m_passedCooldownTime >= m_arrowSprayCooldown)
		{
			m_arrowSprayReady = true;
			m_passedCooldownTime = 0.0f;
		}
	}
}

void AEnemy_Rogue::FireArrow(AActor* a_target)
{
	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), m_projectileSpawnPoint->GetComponentLocation(),FVector(1.0f, 1.0f, 1.0f));

	AEnemy_RogueArrow* arrow = GetWorld()->SpawnActorDeferred<AEnemy_RogueArrow>(m_arrowBP, transform);
	arrow->SetOwnerEnemy(this);
	arrow->SetProjectileSpeed(m_arrowSpeed);
	arrow->SetProjectileLifeSpan(m_arrowLifetime);
	arrow->SetTarget(a_target);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}
