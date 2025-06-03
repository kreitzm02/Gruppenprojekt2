// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerCharDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerCharDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skeletal Mesh", meta = (DisplayName = "Skeletal Mesh"))
	USkeletalMesh* m_Mesh;

	UPROPERTY(EditAnywhere, Category = "Character Settings", meta = (DisplayName = "Display Name"))
	FString m_DisplayName;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Health Points"))
	int32 m_BaseHealthPoints;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Attack Points"))
	int32 m_BaseAttackPoints;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Attack Speed"))
	int32 m_BaseAttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Movement Speed"))
	int32 m_BaseMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Luck"))
	int32 m_BaseLuck;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Defense"))
	int32 m_BaseDefense;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Starting Ability (Placeholder)"))
	int m_StartingAbility;
};
