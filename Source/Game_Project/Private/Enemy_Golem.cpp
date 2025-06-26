// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Golem.h"

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

void AEnemy_Golem::CreateShockwave()
{
	UE_LOG(LogTemp, Warning, TEXT("Shockwave fired"))
}
