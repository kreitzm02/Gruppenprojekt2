// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Warrior.h"

void AEnemy_Warrior::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	m_attackDuration = m_attackAnimation->GetPlayLength();
}

void AEnemy_Warrior::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_Warrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_chargeReady && m_passedCooldownTime <= m_chargeCooldown)
	{
		m_passedCooldownTime += DeltaTime;

		if (m_passedCooldownTime >= m_chargeCooldown)
		{
			m_chargeReady = true;
			m_passedCooldownTime = 0.0f;
		}
	}
}
