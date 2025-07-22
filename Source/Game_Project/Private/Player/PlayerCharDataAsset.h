// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseAbility.h"
#include "MainAbilityContainerDataAsset.h"
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
	float m_BaseHealthPoints;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Attack Damage"))
	float m_BaseAttackPoints;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Attack Speed"))
	float m_BaseAttackSpeed;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Movement Speed"))
	float m_BaseMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Luck"))
	float m_BaseLuck;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Defense"))
	float m_BaseDefense;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (DisplayName = "Base Stamina"))
	float m_BaseStamina;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Starting Ability"))
	UMainAbilityContainerDataAsset* m_StartingAbility;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Debug Ability 1"))
	UMainAbilityContainerDataAsset* m_StartingAbility1Debug;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Debug Ability 2"))
	UMainAbilityContainerDataAsset* m_StartingAbility2Debug;

	UPROPERTY(EditAnywhere, Category = "Placeholders", meta = (DisplayName = "Debug Ability 3"))
	UMainAbilityContainerDataAsset* m_StartingAbility3Debug;

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
