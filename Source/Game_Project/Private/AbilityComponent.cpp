// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

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

	for (auto& abilityClass : m_AbilityClasses)
	{
		if (!abilityClass) continue;
		UBaseAbility* newAbility = NewObject<UBaseAbility>(this, abilityClass);
		m_Abilities.Add(newAbility);
	}
	
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

void UAbilityComponent::TryAddAbility(TSubclassOf<UBaseAbility> a_AbilityClass)
{
	if (!a_AbilityClass || !GetOwner()) return;

	for (UBaseAbility* existingAbility : m_Abilities)
	{
		if (existingAbility && existingAbility->GetClass() == a_AbilityClass) return;
	}

	UBaseAbility* newAbility = NewObject<UBaseAbility>(this, a_AbilityClass);
	if (!newAbility) return;

	m_Abilities.Add(newAbility);
}

