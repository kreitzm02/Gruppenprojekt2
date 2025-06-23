// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <AbilityActions/BaseAbilityAction.h>
#include "TestDataAsset.generated.h"


/**
 * 
 */
UCLASS()
class UTestDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "Enemy Stats", meta = (DisplayName = "Enemy Max Health"))
	int32 m_EnemyMaxHealth;

	UPROPERTY(EditAnywhere, Category = "Enemy Stats", meta = (DisplayName = "Enemy Max Attack Damage"))
	int32 m_EnemyAttackDamage;

	UPROPERTY(EditAnywhere, Category = "Enemy Abilties", meta = (DisplayName = "Enemy Abilities"))
	TArray<UBaseAbilityAction*> m_EnemyAbilityActions;

	UPROPERTY(EditAnywhere, Category = "Enemy Mesh", meta = (DisplayName = "Enemy Mesh"))
	UStaticMesh * m_EnemyMesh;
};
