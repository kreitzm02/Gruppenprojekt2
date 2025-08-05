// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_GolemSmash.h"

void UFSM_GolemSmash::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<AEnemy_Golem>(m_ownerCharacter);
	m_smashAnimation = m_thisEnemy->GetSmashAnimation();
	m_thisEnemy->SetSmashDuration(m_smashAnimation->GetPlayLength());
}

void UFSM_GolemSmash::OnEnter()
{
	Super::OnEnter();

	m_passedTime = 0.0f;
	m_shockwaveStarted = false;
	m_soundStarted = false;
	m_ownerSkeletalMesh->PlayAnimation(m_smashAnimation, false);
	m_thisEnemy->PlaySmashAttackSound(false);
}

void UFSM_GolemSmash::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	m_passedTime += a_deltaTime;

	if (!m_soundStarted && m_passedTime >= m_playSoundAtAnimOffset)
	{
		m_thisEnemy->PlaySmashAttackSound(false);
		m_soundStarted = true;
	}

	if (m_passedTime >= m_shockwaveAtAnimStartOffset && !m_shockwaveStarted)
	{
		m_thisEnemy->CreateShockwave();
		m_shockwaveStarted = true;
	}
}

void UFSM_GolemSmash::OnExit()
{
	Super::OnExit();
	m_thisEnemy->StopOwnSound();
	m_thisEnemy->SetSmashReady(false);
}
