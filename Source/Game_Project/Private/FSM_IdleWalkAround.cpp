// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_IdleWalkAround.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyCharacter.h"

void UFSM_IdleWalkAround::Initialize()
{
    Super::Initialize();
    AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_ownerCharacter);
    m_walkAnimation = enemy->GetWalkAnimation();
	m_walkSpeed = enemy->GetIdleWalkSpeed();
}

void UFSM_IdleWalkAround::OnEnter()
{
	Super::OnEnter();

    //m_ownerSkeletalMesh->PlayAnimation(m_walkAnimation, true);

    //UE_LOG(LogTemp, Warning, TEXT("owner of state: %s\n"), *m_ownerCharacter->GetOwner()->GetName());
    ACharacter* character = Cast<ACharacter>(m_ownerCharacter);
    character->GetCharacterMovement()->MaxWalkSpeed = m_walkSpeed;
    

    //UE_LOG(LogTemp, Warning, TEXT("walk on enter\n"));
    if (m_ownerCharacter != nullptr)
    {
        ChooseRandomNavLocation(m_ownerCharacter);
        //UE_LOG(LogTemp, Warning, TEXT("aactor owner found\n"));
        if (AAIController* AIController = Cast<AAIController>(m_ownerCharacter->GetController()))
        {
            AIController->MoveToLocation(m_targetLocation);
            //UE_LOG(LogTemp, Warning, TEXT("aaicontroller found\n"));
        }
    }
}

void UFSM_IdleWalkAround::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);
   
    //UE_LOG(LogTemp, Warning, TEXT("idle walk around\n"));
    
}

void UFSM_IdleWalkAround::OnExit()
{
	Super::OnExit();

    if (ACharacter* character = Cast<ACharacter>(m_ownerCharacter))
    {
        character->GetCharacterMovement()->StopMovementImmediately();
    }
}


void UFSM_IdleWalkAround::ChooseRandomNavLocation(AActor* a_owner)
{
    if (!a_owner) return;

    UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(a_owner->GetWorld());
    if (!navSys) return;

    FVector origin = a_owner->GetActorLocation();
    FNavLocation navLocation;

    float minRadius = 500.0f;
    float maxRadius = 1000.0f;
    int maxAttempts = 10;

    //bool found = navSys->GetRandomReachablePointInRadius(origin, 1000.0f, navLocation);

    for (int i = 0; i < maxAttempts; ++i)
    {
        if (navSys->GetRandomReachablePointInRadius(origin, maxRadius, navLocation))
        {
            float distance = FVector::Dist(navLocation.Location, origin);
            if (distance >= minRadius)
            {
                m_targetLocation = navLocation.Location;
                m_ownerSkeletalMesh->PlayAnimation(m_walkAnimation, true);
                return;
            }
        }
    }
}