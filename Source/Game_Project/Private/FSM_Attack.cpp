// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_Attack.h"
#include "Enemy_Warrior.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSM_Attack::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_Warrior>(m_ownerPawn);
	m_attackAnimation = m_thisEnemy->GetAttackAnimation();
}

void UFSM_Attack::OnEnter()
{
	if (ACharacter* character = Cast<ACharacter>(m_ownerPawn))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}
	Super::OnEnter();
	m_ownerSkeletalMesh->PlayAnimation(m_attackAnimation,true);
	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UFSM_Attack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);
}

void UFSM_Attack::OnExit()
{
	Super::OnExit();

	m_thisEnemy->GetWeaponHitbox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}