// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbility.h"
#include "TimerManager.h"

void UBaseAbility::UseAbility(AActor* a_AbilityUser)
{
	if (!CanUse() || !a_AbilityUser) return;

	StartCooldown(a_AbilityUser->GetWorld());
}

void UBaseAbility::EquipAbility(AActor* a_AbilityUser)
{
}

void UBaseAbility::StartCooldown(UWorld* a_World)
{
	if (!a_World || m_CooldownTime <= 0.0f) return;

	m_IsOnCooldown = true;

	FTimerDelegate resetDel = FTimerDelegate::CreateLambda([this]() { m_IsOnCooldown = false; });

	FTimerHandle timerHandle;
	a_World->GetTimerManager().SetTimer(timerHandle, resetDel, m_CooldownTime, false);
}
