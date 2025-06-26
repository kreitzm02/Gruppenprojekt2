// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Rogue.h"

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

void AEnemy_Rogue::FireArrow(FVector a_dirction)
{
	UE_LOG(LogTemp, Warning, TEXT("Firing arrow!"));
}
