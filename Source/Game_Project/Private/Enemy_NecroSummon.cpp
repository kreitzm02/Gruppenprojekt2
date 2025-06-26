// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_NecroSummon.h"

void AEnemy_NecroSummon::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	if (m_riseAnimation)
	{
		m_riseDuration = m_riseAnimation->GetPlayLength();
	}
	m_weaponHitbox->SetActive(false);
	m_weaponMesh->SetActive(false);
}


void AEnemy_NecroSummon::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_NecroSummon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
