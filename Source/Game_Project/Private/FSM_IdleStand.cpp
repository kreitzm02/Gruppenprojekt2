// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_IdleStand.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyCharacter.h"

void UFSM_IdleStand::Initialize()
{
	Super::Initialize();
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_ownerCharacter);
	m_idleAnimation = enemy->GetIdleAnimation();
}

void UFSM_IdleStand::OnEnter()
{
	Super::OnEnter();

	m_ownerSkeletalMesh->PlayAnimation(m_idleAnimation, true);

	if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}
}

void UFSM_IdleStand::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);
	//UE_LOG(LogTemp, Warning, TEXT("idle stand\n"));
}

void UFSM_IdleStand::OnExit()
{
	Super::OnExit();
}
