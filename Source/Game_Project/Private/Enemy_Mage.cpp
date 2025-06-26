// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Mage.h"

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

void AEnemy_Mage::FireProjectile(FVector a_direction)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Mage Projectile"))
}

void AEnemy_Mage::FireFireball(ACharacter* a_playerToFollow)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Mage Fireball"))
}
