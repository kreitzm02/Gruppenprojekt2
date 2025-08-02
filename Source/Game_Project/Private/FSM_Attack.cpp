// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_Attack.h"
#include "Enemy_Warrior.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSM_Attack::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_Warrior>(m_ownerCharacter);
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
}

void UFSM_Attack::OnEnter()
{
	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}
	Super::OnEnter();
	m_animationDuration = m_attackAnimation->GetPlayLength();
	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UFSM_Attack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	m_passedTime += a_deltaTime;

	if (!m_animationStarted)
	{
		m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, false);

		m_thisEnemy->PlayBasicAttackSound(false);

		m_passedTime = 0.0f;

		m_animationStarted = true;
	}

	if (m_animationStarted && m_passedTime >= m_animationDuration)
	{
		m_animationStarted = false;
	}
}

void UFSM_Attack::OnExit()
{
	Super::OnExit();
	m_thisEnemy->StopOwnSound();
	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}