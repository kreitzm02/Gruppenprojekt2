// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStateMachineComponent.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UE5MainStreamObjectVersion.h"

// Sets default values for this component's properties
UEnemyStateMachineComponent::UEnemyStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeTMap();

	for (FTargetStateWithCondition a_transition : stateStructure[currentState->GetClass()])
	{
		a_transition.targetCondition->Initialize();
	}

	currentState->OnEnter();
}


// Called every frame
void UEnemyStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentState->OnUpdate(DeltaTime);
	TransitionHandler();
}


void UEnemyStateMachineComponent::InitializeTMap()
{
	for (FStateData a_stateData : stateData->GetStateData())
	{
		TSubclassOf<UBaseEnemyState> originState = a_stateData.originState->GetClass();
	
		TArray<FTargetStateWithCondition> targetStateAndCond;

		for (FTargetStateWithCondition a_targetStateAndCond : a_stateData.targetStateWithCond)
		{
			UBaseStateTransition* conditionTemp = DuplicateObject<UBaseStateTransition>(a_targetStateAndCond.targetCondition, this);
			UBaseEnemyState* stateTemp = DuplicateObject<UBaseEnemyState>(a_targetStateAndCond.targetState, this);

			stateTemp->Initialize(Cast<APawn>(GetOwner()));


			ownedTransitions.Add(conditionTemp);
			ownedStates.Add(stateTemp);

			FTargetStateWithCondition targetStateAndTransTemp = FTargetStateWithCondition();
			targetStateAndTransTemp.targetCondition = conditionTemp;
			targetStateAndTransTemp.targetState = stateTemp;

			targetStateAndCond.Add(targetStateAndTransTemp);

			if (initialState->GetClass() == stateTemp->GetClass())
			{
				currentState = stateTemp;
				UE_LOG(LogTemp, Warning, TEXT("currentState set\n"));
			}
		}

		stateStructure.Add(originState,targetStateAndCond);
		
	}
}

void UEnemyStateMachineComponent::TransitionHandler()
{
	for (FTargetStateWithCondition a_transition : stateStructure[currentState->GetClass()])
	{
		if (a_transition.targetCondition->IsConditionMet())
		{
			currentState->OnExit();
			currentState = a_transition.targetState;
			currentState->OnEnter();
			for (FTargetStateWithCondition a_transition2 : stateStructure[currentState->GetClass()])
			{
				a_transition2.targetCondition->Initialize();
			}
			//currentState->OnEnter();
			break;
		}
	}
}

