// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSM_BossWarriorAttack::Initialize()
{
	Super::Initialize();

	m_thisEnemy = Cast<ABossEnemy_Warrior>(m_ownerCharacter);

	if (!m_thisEnemy) return;

	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
}

void UFSM_BossWarriorAttack::OnEnter()
{

	if (!m_thisEnemy) return;

	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}
	Super::OnEnter();
	m_animationDuration = m_attackAnimation->GetPlayLength();
	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UFSM_BossWarriorAttack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);

	if (!m_thisEnemy) return;

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

void UFSM_BossWarriorAttack::OnExit()
{
	Super::OnExit();

	if (!m_thisEnemy) return;

	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}