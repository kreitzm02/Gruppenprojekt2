// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMData.h"
#include "Components/ActorComponent.h"
#include "EnemyStateMachineComponent.generated.h"

class UBaseEnemyState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_PROJECT_API UEnemyStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Instanced)
	UBaseEnemyState* initialState = nullptr;

	UPROPERTY(EditAnywhere)
	UFSMData* stateData = nullptr;

	UPROPERTY()
	UBaseEnemyState* currentState = nullptr;
	//needed otherwise garbage collector deletes stuff
	UPROPERTY()
	TArray<UBaseStateTransition*> ownedTransitions;
	//needed otherwise garbage collector deletes stuff
	UPROPERTY()
	TArray<UBaseEnemyState*> ownedStates;

	TMap<TSubclassOf<UBaseEnemyState>, TArray<FTargetStateWithCondition>> stateStructure;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeTMap();

	void TransitionHandler();
};
