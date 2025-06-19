// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_CondPlayerInAtkRange.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "EnemyCharacter.h"


void UFSM_CondPlayerInAtkRange::Initialize()
{
	owner = Cast<AActor>(GetOuter()->GetOuter());

	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(owner);
	attackRange = enemy->GetAttackRange();
}

void UFSM_CondPlayerInAtkRange::ResetCondition()
{

}

bool UFSM_CondPlayerInAtkRange::IsConditionMet(float a_deltaTime)
{
	if (owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("In Range Cond AActor not found!"))
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
		FCollisionShape::MakeSphere(attackRange),
		queryParams
	);

	DrawDebugSphere(
		owner->GetWorld(),
		owner->GetActorLocation(),
		attackRange,
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