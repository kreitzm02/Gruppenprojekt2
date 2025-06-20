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

	for (FTargetStateWithCondition a_transition : m_stateStructure[m_currentState->GetClass()])
	{
		a_transition.m_targetCondition->Initialize();
	}

	m_currentState->OnEnter();
}


// Called every frame
void UFSM_EnemyStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_isDead)
	{
		m_currentState->OnUpdate(DeltaTime);
		TransitionHandler(DeltaTime);
	}
	
}


void UFSM_EnemyStateMachineComponent::InitializeTMap()
{
	for (FStateData a_stateData : m_stateData->GetStateData())
	{
		TSubclassOf<UFSM_BaseEnemyState> originState = a_stateData.m_originState->GetClass();
		TArray<FTargetStateWithCondition> targetStateAndCond;

		for (FTargetStateWithCondition a_targetStateAndCond : a_stateData.m_targetStateWithCond)
		{
			UFSM_BaseStateTransition* conditionTemp = DuplicateObject<UFSM_BaseStateTransition>(a_targetStateAndCond.m_targetCondition, this);
			UFSM_BaseEnemyState* stateTemp = DuplicateObject<UFSM_BaseEnemyState>(a_targetStateAndCond.m_targetState, this);

			stateTemp->Initialize();
			conditionTemp->Initialize();


			m_ownedTransitions.Add(conditionTemp);
			m_ownedStates.Add(stateTemp);

			FTargetStateWithCondition targetStateAndTransTemp = FTargetStateWithCondition();
			targetStateAndTransTemp.m_targetCondition = conditionTemp;
			targetStateAndTransTemp.m_targetState = stateTemp;

			targetStateAndCond.Add(targetStateAndTransTemp);

			if (m_initialState->GetClass() == stateTemp->GetClass())
			{
				m_currentState = stateTemp;
				UE_LOG(LogTemp, Warning, TEXT("m_currentState set\n"));
			}
		}

		m_stateStructure.Add(originState,targetStateAndCond);
		
	}
}

void UFSM_EnemyStateMachineComponent::TransitionHandler(float a_deltaTime)
{
	for (FTargetStateWithCondition a_transition : m_stateStructure[m_currentState->GetClass()])
	{
		if (a_transition.m_targetCondition->IsConditionMet(a_deltaTime))
		{
			m_currentState->OnExit();
			m_currentState = a_transition.m_targetState;
			m_currentState->OnEnter();
			if (m_currentState->IsA(UFSM_Dead::StaticClass()))
			{
				AEnemyCharacter* ownerCharacter = Cast<AEnemyCharacter>(GetOwner());
				ownerCharacter->SetDeathState(true);
				m_isDead = true;
			}
			else
			{
				for (FTargetStateWithCondition a_transition2 : m_stateStructure[m_currentState->GetClass()])
				{
					a_transition2.m_targetCondition->ResetCondition();
				}
			}
			
			//m_currentState->OnEnter();
			break;
		}
	}
}

