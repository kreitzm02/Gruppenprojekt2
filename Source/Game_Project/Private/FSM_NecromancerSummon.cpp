// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_NecromancerSummon.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UFSM_NecromancerSummon::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<AEnemy_Necromancer>(m_ownerCharacter);
	m_summonAnimation = m_thisEnemy->GetSummonAnimation();
}

void UFSM_NecromancerSummon::OnEnter()
{
	Super::OnEnter();

	m_thisEnemy->GetCharacterMovement()->StopMovementImmediately();
	m_ownerSkeletalMesh->PlayAnimation(m_summonAnimation, true);
	m_thisEnemy->PlaySummonSound(false);
	m_thisEnemy->Summon();
}

void UFSM_NecromancerSummon::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);
}

void UFSM_NecromancerSummon::OnExit()
{
	Super::OnExit();

	m_thisEnemy->SetSummonReady(false);
}
