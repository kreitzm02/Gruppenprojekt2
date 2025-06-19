// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionPlayerInRange.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "EnemyCharacter.h"


void UFSM_ConditionPlayerInRange::Initialize()
{
	owner = Cast<AActor>(GetOuter()->GetOuter());
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(owner);
	if (checkForInRange)
	{
		detectionRadius = enemy->GetPlayerDetectionRadius();
	}
	else
	{
		detectionRadius = enemy->GetPlayerChaseRadius();
	}
}

void UFSM_ConditionPlayerInRange::ResetCondition()
{

}

bool UFSM_ConditionPlayerInRange::IsConditionMet(float a_deltaTime)
{
	if (owner == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("In Range Cond AActor not found!"))
		return false;
	}
	TArray<FOverlapResult> overlaps;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(owner);
	
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	
	bool hasOverlap = owner->GetWorld()->OverlapMultiByObjectType(
		overlaps,
		owner->GetActorLocation(),
		FQuat::Identity,
		objectQueryParams,
		FCollisionShape::MakeSphere(detectionRadius),
		queryParams
	);

	DrawDebugSphere(
		owner->GetWorld(),
		owner->GetActorLocation(),
		detectionRadius,
		16,
		FColor::Red, 
		false,        
		0.1f,          
		0,             
		1.0f           
	);

	if (checkForInRange)
	{
		if (!hasOverlap) return false;
		else return true;
	}
	else
	{
		if (!hasOverlap) return true;
		else return false;
	}

	
}

