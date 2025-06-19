// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_IdleStand.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyCharacter.h"

void UFSM_IdleStand::Initialize()
{
	Super::Initialize();
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(ownerPawn);
	idleAnimation = enemy->GetIdleAnimation();
}

void UFSM_IdleStand::OnEnter()
{
	Super::OnEnter();

	ownerSkeletalMesh->PlayAnimation(idleAnimation, true);

	if (ACharacter* character = Cast<ACharacter>(ownerPawn))
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
