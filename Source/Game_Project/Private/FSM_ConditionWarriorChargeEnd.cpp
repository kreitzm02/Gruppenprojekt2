// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionWarriorChargeEnd.h"
#include "Engine/OverlapResult.h"

void UFSM_ConditionWarriorChargeEnd::Initialize()
{
	Super::Initialize();
	//owner = Cast<AActor>(GetOuter()->GetOuter());
	thisWarrior = Cast<AEnemy_Warrior>(GetOuter()->GetOuter());
	attackRange = thisWarrior->GetAttackRange();
	chargeDuration = thisWarrior->GetChargeDuration();
}

void UFSM_ConditionWarriorChargeEnd::ResetCondition()
{
	Super::ResetCondition();
	chargeCurrentDuration = 0.0f;
}

bool UFSM_ConditionWarriorChargeEnd::IsConditionMet(float a_deltaTime)
{
	chargeCurrentDuration += a_deltaTime;

	TArray<FOverlapResult> overlaps;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(thisWarrior);

	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

	bool hasOverlap = thisWarrior->GetWorld()->OverlapMultiByObjectType(
		overlaps,
		thisWarrior->GetActorLocation(),
		FQuat::Identity,
		objectQueryParams,
		FCollisionShape::MakeSphere(attackRange),
		queryParams
	);
	for (FOverlapResult& overlap : overlaps)
	{
		AActor* actor = overlap.GetActor();
		if (actor && actor->IsA(ACharacter::StaticClass()))
		{
			player = Cast<ACharacter>(actor);
		}
	}

	if (hasOverlap || chargeCurrentDuration >= chargeDuration) return true;
	else return false;

}