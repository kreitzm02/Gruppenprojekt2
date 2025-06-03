// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleStand.h"

void UIdleStand::OnEnter()
{
	ownerSkeletalMesh->PlayAnimation(idleAnimation, true);
}

void UIdleStand::OnUpdate(float a_deltatime)
{
	UE_LOG(LogTemp, Warning, TEXT("idle stand\n"));
}

void UIdleStand::OnExit()
{
	
}
