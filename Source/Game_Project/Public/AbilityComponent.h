// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAbility.h"
#include "AbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_PROJECT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UBaseAbility>> m_AbilityClasses;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ActivateAbility(int32 a_Index);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void EquipAbility(int32 a_Index);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryAddAbility(TSubclassOf<UBaseAbility> a_AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAllAbilities();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(TSubclassOf<UBaseAbility> a_AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbilityFromIndex(int a_Index);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	float GetRemainingCooldownFromAbility(int32 a_Index);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FText GetAbilityName(int32 a_Index);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UTexture2D* GetAbilityIcon(int32 a_Index);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	float GetAbilityCooldown(int32 a_Index);

	UPROPERTY(EditAnywhere)
	TArray<UBaseAbility*> m_Abilities;
		
};
