// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseAbility.h"
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

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Attack Damage"))
	int32 m_BaseAttackPoints;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Attack Speed"))
	int32 m_BaseAttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Movement Speed"))
	int32 m_BaseMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Luck"))
	int32 m_BaseLuck;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Defense"))
	int32 m_BaseDefense;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Starting Ability"))
	TSubclassOf<UBaseAbility> m_StartingAbility;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Debug Ability 1"))
	TSubclassOf<UBaseAbility> m_StartingAbility1Debug;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Debug Ability 2"))
	TSubclassOf<UBaseAbility> m_StartingAbility2Debug;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Debug Ability 3"))
	TSubclassOf<UBaseAbility> m_StartingAbility3Debug;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (DisplayName = "Idle Animation"))
	UAnimSequence* m_IdleAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (DisplayName = "Walk Animation"))
	UAnimSequence* m_WalkAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (DisplayName = "Sprint Animation"))
	UAnimSequence* m_SprintAnim;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (DisplayName = "1H Attack Animation Variant A"))
	UAnimSequence* m_1HAttackVar1Anim;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (DisplayName = "1H Attack Animation Variant B"))
	UAnimSequence* m_1HAttackVar2Anim;

	UPROPERTY(EditAnywhere, Category = "Animations", meta = (DisplayName = "1H Attack Animation Variant C"))
	UAnimSequence* m_1HAttackVar3Anim;
};
