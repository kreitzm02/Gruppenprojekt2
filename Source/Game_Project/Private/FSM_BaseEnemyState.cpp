// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_BaseEnemyState.h"


void UFSM_BaseEnemyState::Initialize()
{
	ownerPawn = Cast<APawn>(GetOuter()->GetOuter());
	ownerSkeletalMesh = ownerPawn->FindComponentByClass<USkeletalMeshComponent>();
}

void UFSM_BaseEnemyState::OnEnter()
{
	
}

void UFSM_BaseEnemyState::OnUpdate(float a_deltaTime)
{
	
}

void UFSM_BaseEnemyState::OnExit()
{
	
}
