// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyState.h"

void UBaseEnemyState::Initialize(APawn* a_pawn)
{
	ownerPawn = a_pawn;
	ownerSkeletalMesh = ownerPawn->FindComponentByClass<USkeletalMeshComponent>();
}


void UBaseEnemyState::OnEnter()
{
	
}

void UBaseEnemyState::OnUpdate(float a_deltaTime)
{
	
}

void UBaseEnemyState::OnExit()
{
	
}
