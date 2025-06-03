// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleWalkAround.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UIdleWalkAround::OnEnter()
{
	Super::OnEnter();

    ownerSkeletalMesh->PlayAnimation(walkAnimation, true);

    UE_LOG(LogTemp, Warning, TEXT("owner of state: %s\n"), *ownerPawn->GetOwner()->GetName());
    if (ACharacter* character = Cast<ACharacter>(ownerPawn))
    {
        character->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
    }

    moveDuration = FMath::FRandRange(10.0f, 12.0f);
    UE_LOG(LogTemp, Warning, TEXT("walk on enter\n"));
    if (ownerPawn != nullptr)
    {
        ChooseRandomNavLocation(ownerPawn);
        UE_LOG(LogTemp, Warning, TEXT("aactor owner found\n"));
        if (AAIController* AIController = Cast<AAIController>(ownerPawn->GetController()))
        {
            AIController->MoveToLocation(targetLocation);
            UE_LOG(LogTemp, Warning, TEXT("aaicontroller found\n"));
        }
    }
}

void UIdleWalkAround::OnUpdate(float a_deltaTime)
{
	Super::OnUpdate(a_deltaTime);
   
    UE_LOG(LogTemp, Warning, TEXT("idle walk around\n"));
    
}

void UIdleWalkAround::OnExit()
{
	Super::OnExit();

    if (AActor* owner = Cast<AActor>(GetOuter()))
    {
        if (AAIController* aiController = Cast<AAIController>(Cast<APawn>(owner)->GetController()))
        {
            aiController->StopMovement();
        }
    }
}


void UIdleWalkAround::ChooseRandomNavLocation(AActor* a_owner)
{
    if (!a_owner) return;

    UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(a_owner->GetWorld());
    if (!navSys) return;

    FVector origin = a_owner->GetActorLocation();
    FNavLocation navLocation;

    float minRadius = 300.0f;
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
                targetLocation = navLocation.Location;
                return;
            }
        }
    }
    

    //if (found)
    //{
    //    targetLocation = navLocation.Location;
    //}
    //else
    //{
    //    targetLocation = origin;
    //}
}