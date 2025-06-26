// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbility.h"
#include "TimerManager.h"
#include <Player/PlayerCharacter.h>


void UBaseAbility::UseAbility(AActor* a_AbilityUser)
{
	if (!CanUse() || !a_AbilityUser) return;

	StartCooldown(a_AbilityUser->GetWorld());

	for (int i = 0; i < m_AbilityActions.Num(); i++)
	{
		m_AbilityActions[i]->PlayAbilityAction(a_AbilityUser);
	}
}

void UBaseAbility::EquipAbility(AActor* a_AbilityUser)
{ 
	if (!m_IsInitialized)
	{
		m_IsInitialized = true;
		m_ConstCooldownTime = m_CooldownTime;
	}

	for (int i = 0; i < m_AbilityActions.Num(); i++)
	{
		m_AbilityActions[i]->PrepareAbilityAction(a_AbilityUser);
	}

	if (APlayerCharacter* player = Cast<APlayerCharacter>(a_AbilityUser))
	{
		m_CooldownTime = m_ConstCooldownTime * (100 - player->GetPlayerAttackSpeed()) / 100;
	}
}

void UBaseAbility::StartCooldown(UWorld* a_World)
{
	if (!a_World || m_CooldownTime <= 0.0f) return;

	m_IsOnCooldown = true;

	FTimerDelegate resetDel = FTimerDelegate::CreateLambda([this]() { m_IsOnCooldown = false; });

	FTimerHandle timerHandle;
	a_World->GetTimerManager().SetTimer(timerHandle, resetDel, m_CooldownTime, false);
}
