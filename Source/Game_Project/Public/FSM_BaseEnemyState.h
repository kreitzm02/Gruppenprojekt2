// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UObject/NoExportTypes.h"
#include "FSM_BaseEnemyState.generated.h"

class UFSM_BaseStateTransition;
/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable)
class GAME_PROJECT_API UFSM_BaseEnemyState : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize();

    virtual void OnEnter();
    
    virtual void OnExit();
    
    virtual void OnUpdate(float a_deltaTime);

	//void SetOwnerPawn(APawn* a_pawn)
	//{
	//	m_ownerCharacter = a_pawn;
	//}

	//APawn* GetOwnerPawn()
	//{
	//	return m_ownerCharacter;
	//}
protected:
	UPROPERTY()
	ACharacter* m_ownerCharacter;
	UPROPERTY()
	USkeletalMeshComponent* m_ownerSkeletalMesh;

};
