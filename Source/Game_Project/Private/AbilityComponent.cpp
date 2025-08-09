// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Game_GameInstance.h>

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	//for (auto& abilityClass : m_AbilityClasses)
	//{
	//	if (!abilityClass) continue;
	//	UBaseAbility* newAbility = NewObject<UBaseAbility>(this, abilityClass);
	//	m_Abilities.Add(newAbility);
	//}
	
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilityComponent::ActivateAbility(int32 a_Index)
{
	if (m_Abilities.IsValidIndex(a_Index) && m_Abilities[a_Index]->CanUse())
	{
		m_Abilities[a_Index]->UseAbility(GetOwner());
	}
}

void UAbilityComponent::EquipAbility(int32 a_Index)
{
	if (m_Abilities.IsValidIndex(a_Index))
	{
		m_Abilities[a_Index]->EquipAbility(GetOwner());
	}
}

bool UAbilityComponent::TryAddAbility(TSubclassOf<UBaseAbility> a_AbilityClass)
{
	if (!a_AbilityClass || !GetOwner()) return false;
	UE_LOG(LogTemp, Warning, TEXT("TryAddAbility was called"));
	for (UBaseAbility* existingAbility : m_Abilities)
	{
		if (existingAbility && existingAbility->GetClass() == a_AbilityClass) return false;
	}

	UBaseAbility* newAbility = NewObject<UBaseAbility>(this, a_AbilityClass);
	if (!newAbility) return false;
	UE_LOG(LogTemp, Warning, TEXT("New ability added"));
	m_Abilities.Add(newAbility);
	m_AbilityClasses.Add(a_AbilityClass);
	return true;
}

bool UAbilityComponent::TryReplaceAbility(TSubclassOf<UBaseAbility> a_abilityClassToReplace, TSubclassOf<UBaseAbility> a_newAbilityClass)
{
	if (!a_abilityClassToReplace || !a_newAbilityClass || !GetOwner()) return false;
	UE_LOG(LogTemp, Warning, TEXT("TryAddAbility was called"));

	int index = 0;

	for (UBaseAbility* existingAbility : m_Abilities)
	{
		if (existingAbility->GetClass() == a_abilityClassToReplace)
		{
			break;
		}
		else
		{
			index++;
		}
	}

	if (index >= m_Abilities.Num()) return false;

	UBaseAbility* newAbility = NewObject<UBaseAbility>(this, a_newAbilityClass);
	if (!newAbility) return false;
	UE_LOG(LogTemp, Warning, TEXT("Ability Replaced"));
	m_Abilities[index] = newAbility;
	m_AbilityClasses[index] = (a_newAbilityClass);
	return true;
}

bool UAbilityComponent::TryReplaceAbilityFromIndex(int a_indexToReplace, TSubclassOf<UBaseAbility> a_newAbilityClass)
{
	if (a_indexToReplace >= m_Abilities.Num() || !a_newAbilityClass || !GetOwner()) return false;
	UE_LOG(LogTemp, Warning, TEXT("TryAddAbility was called"));

	UBaseAbility* newAbility = NewObject<UBaseAbility>(this, a_newAbilityClass);
	if (!newAbility) return false;
	UE_LOG(LogTemp, Warning, TEXT("Ability Replaced"));
	m_Abilities[a_indexToReplace] = newAbility;
	m_AbilityClasses[a_indexToReplace] = (a_newAbilityClass);
	return true;
}


void UAbilityComponent::RemoveAllAbilities()
{
	for (UBaseAbility* ability : m_Abilities) ability->ForceEndAbility(this->GetOwner());
	m_Abilities.Empty();
	m_AbilityClasses.Empty();
}

void UAbilityComponent::RemoveAbility(TSubclassOf<UBaseAbility> a_AbilityClass)
{
	for (int i = m_Abilities.Num() - 1; i >= 0; i--)
	{
		if (m_Abilities[i] && m_Abilities[i]->GetClass() == a_AbilityClass)
		{
			m_Abilities.RemoveAt(i);
			m_AbilityClasses.RemoveAt(i);
			return;
		}
	}
}

void UAbilityComponent::RemoveAbilityFromIndex(int a_Index)
{
	if (a_Index > m_Abilities.Num()) return;

	m_Abilities.RemoveAt(a_Index);
	m_AbilityClasses.RemoveAt(a_Index);
}

float UAbilityComponent::GetRemainingCooldownFromAbility(int32 a_Index)
{
	if (m_Abilities.IsValidIndex(a_Index))
		return m_Abilities[a_Index]->GetCooldownTime(GetOwner()->GetWorld());
	else return -1;
}

FText UAbilityComponent::GetAbilityName(int32 a_Index)
{
	if (m_Abilities.IsValidIndex(a_Index))
		return m_Abilities[a_Index]->m_AbilityName;
	else return FText::FromString(TEXT("Invalid Ability Name"));
}

UTexture2D* UAbilityComponent::GetAbilityIcon(int32 a_Index)
{
	if (m_Abilities.IsValidIndex(a_Index))
		return m_Abilities[a_Index]->m_AbilityIcon;
	else return nullptr;
}

float UAbilityComponent::GetAbilityCooldown(int32 a_Index)
{
	if (m_Abilities.IsValidIndex(a_Index))
		return m_Abilities[a_Index]->m_CooldownTime;
	else return -1;
}

