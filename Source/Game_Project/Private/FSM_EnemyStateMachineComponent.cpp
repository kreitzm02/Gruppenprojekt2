// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_EnemyStateMachineComponent.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UE5MainStreamObjectVersion.h"
#include "FSM_Dead.h"
#include "EnemyCharacter.h"

// Sets default values for this component's properties
UFSM_EnemyStateMachineComponent::UFSM_EnemyStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFSM_EnemyStateMachineComponent::BeginPlay()
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
void UFSM_EnemyStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isDead)
	{
		currentState->OnUpdate(DeltaTime);
		TransitionHandler(DeltaTime);
	}
	
}


void UFSM_EnemyStateMachineComponent::InitializeTMap()
{
	for (FStateData a_stateData : stateData->GetStateData())
	{
		TSubclassOf<UFSM_BaseEnemyState> originState = a_stateData.originState->GetClass();
		TArray<FTargetStateWithCondition> targetStateAndCond;

		for (FTargetStateWithCondition a_targetStateAndCond : a_stateData.targetStateWithCond)
		{
			UFSM_BaseStateTransition* conditionTemp = DuplicateObject<UFSM_BaseStateTransition>(a_targetStateAndCond.targetCondition, this);
			UFSM_BaseEnemyState* stateTemp = DuplicateObject<UFSM_BaseEnemyState>(a_targetStateAndCond.targetState, this);

			stateTemp->Initialize();
			conditionTemp->Initialize();


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

void UFSM_EnemyStateMachineComponent::TransitionHandler(float a_deltaTime)
{
	for (FTargetStateWithCondition a_transition : stateStructure[currentState->GetClass()])
	{
		if (a_transition.targetCondition->IsConditionMet(a_deltaTime))
		{
			currentState->OnExit();
			currentState = a_transition.targetState;
			currentState->OnEnter();
			if (currentState->IsA(UFSM_Dead::StaticClass()))
			{
				AEnemyCharacter* ownerCharacter = Cast<AEnemyCharacter>(GetOwner());
				ownerCharacter->SetDeathState(true);
				isDead = true;
			}
			else
			{
				for (FTargetStateWithCondition a_transition2 : stateStructure[currentState->GetClass()])
				{
					a_transition2.targetCondition->ResetCondition();
				}
			}
			
			//currentState->OnEnter();
			break;
		}
	}
}

