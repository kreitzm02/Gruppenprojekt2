// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation States")
	bool m_IsWalking;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation States")
	bool m_IsRunning;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation States")
	float m_BlendWeight = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation States")
	bool m_PlayerAlive = true;
};
