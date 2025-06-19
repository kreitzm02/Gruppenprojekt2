// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_Attack.h"
#include "Enemy_Warrior.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSM_Attack::Initialize()
{
	Super::Initialize();
	AEnemy_Warrior* enemy = Cast<AEnemy_Warrior>(ownerPawn);
	attackAnimation = enemy->GetAttackAnimation();
}

void UFSM_Attack::OnEnter()
{
	if (ACharacter* character = Cast<ACharacter>(ownerPawn))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}
	Super::OnEnter();
	ownerSkeletalMesh->PlayAnimation(attackAnimation,true);
}

void UFSM_Attack::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);
}

void UFSM_Attack::OnExit()
{
	Super::OnExit();
}