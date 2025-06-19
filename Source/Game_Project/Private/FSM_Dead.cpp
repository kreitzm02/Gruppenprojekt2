// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_Dead.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyCharacter.h"

void UFSM_Dead::Initialize()
{
	Super::Initialize();
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(ownerPawn);
	deathAnimation = enemy->GetDeathAnimation();
}

void UFSM_Dead::OnEnter()
{
	Super::OnEnter();

	ownerSkeletalMesh->PlayAnimation(deathAnimation, false);

	if (ACharacter* character = Cast<ACharacter>(ownerPawn))
	{
		character->GetCharacterMovement()->StopMovementImmediately();
	}
}

void UFSM_Dead::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);
}

void UFSM_Dead::OnExit()
{
	Super::OnExit();
}