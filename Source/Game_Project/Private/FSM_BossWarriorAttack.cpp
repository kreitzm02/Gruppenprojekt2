// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BossWarriorAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSM_BossWarriorAttack::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<ABossEnemy_Warrior>(m_ownerCharacter);
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
}

void UFSM_BossWarriorAttack::OnEnter()
{
	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}
	Super::OnEnter();
	m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation, true);
	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UFSM_BossWarriorAttack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);
}

void UFSM_BossWarriorAttack::OnExit()
{
	Super::OnExit();

	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}