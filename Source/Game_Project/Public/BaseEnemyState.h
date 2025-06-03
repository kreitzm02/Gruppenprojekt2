// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseEnemyState.generated.h"

class UBaseStateTransition;
/**
 * 
 */
UCLASS(EditInlineNew)
class GAME_PROJECT_API UBaseEnemyState : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(APawn* a_pawn);

    virtual void OnEnter();
    
    virtual void OnExit();
    
    virtual void OnUpdate(float a_deltaTime);

	//void SetOwnerPawn(APawn* a_pawn)
	//{
	//	ownerPawn = a_pawn;
	//}

	//APawn* GetOwnerPawn()
	//{
	//	return ownerPawn;
	//}
protected:
	APawn* ownerPawn;
	USkeletalMeshComponent* ownerSkeletalMesh;

};
