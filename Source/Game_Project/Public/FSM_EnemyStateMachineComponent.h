// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMData.h"
#include "Components/ActorComponent.h"
#include "FSM_EnemyStateMachineComponent.generated.h"

class UFSM_BaseEnemyState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_PROJECT_API UFSM_EnemyStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSM_EnemyStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool m_isDead = false;

	UPROPERTY(EditAnywhere, Instanced)
	UFSM_BaseEnemyState* m_initialState = nullptr;

	UPROPERTY(EditAnywhere)
	UFSMData* m_stateData = nullptr;

	UPROPERTY()
	UFSM_BaseEnemyState* m_currentState = nullptr;

	//needed otherwise garbage collector deletes stuff
	UPROPERTY()
	TArray<UFSM_BaseStateTransition*> m_ownedTransitions;
	//needed otherwise garbage collector deletes stuff
	UPROPERTY()
	TArray<UFSM_BaseEnemyState*> m_ownedStates;

	TMap<TSubclassOf<UFSM_BaseEnemyState>, TArray<FTargetStateWithCondition>> m_stateStructure;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeTMap();

	void TransitionHandler(float a_deltaTime);
};
