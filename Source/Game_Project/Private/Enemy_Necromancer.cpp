// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Necromancer.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Summon Necro Spawns"))
}
