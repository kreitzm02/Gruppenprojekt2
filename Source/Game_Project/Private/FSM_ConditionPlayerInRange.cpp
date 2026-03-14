// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_ConditionPlayerInRange.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "EnemyCharacter.h"


void UFSM_ConditionPlayerInRange::Initialize()
{
	m_owner = Cast<AActor>(GetOuter()->GetOuter());
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_owner);
	if (m_checkForInRange)
	{
		m_detectionRadius = enemy->GetPlayerDetectionRadius();
	}
	else
	{
		m_detectionRadius = enemy->GetPlayerChaseRadius();
	}
}

void UFSM_ConditionPlayerInRange::ResetCondition()
{
	m_target = nullptr;
	
	if (Cast<AEnemyCharacter>(m_owner)->GetCurrentTarget() != nullptr)
	{
		m_target = Cast<AEnemyCharacter>(m_owner)->GetCurrentTarget();
		return;
	}
	
	if (m_target == nullptr)
	{
		SearchNearestPlayer();
	}
}

bool UFSM_ConditionPlayerInRange::IsConditionMet(float a_deltaTime)
{
	if (m_owner == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("In Range Cond AActor not found!"))
		return false;
	}
	//TArray<FOverlapResult> overlaps;
	//FCollisionQueryParams queryParams;
	//queryParams.AddIgnoredActor(m_owner);
	//
	//FCollisionObjectQueryParams objectQueryParams;
	//objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	//
	//bool hasOverlap = m_owner->GetWorld()->OverlapMultiByObjectType(
	//	overlaps,
	//	m_owner->GetActorLocation(),
	//	FQuat::Identity,
	//	objectQueryParams,
	//	FCollisionShape::MakeSphere(m_detectionRadius),
	//	queryParams
	//);

	if (m_enableDebug)
	DrawDebugSphere(
		m_owner->GetWorld(),
		m_owner->GetActorLocation(),
		m_detectionRadius,
		16,
		FColor::Red, 
		false,        
		0.1f,          
		0,             
		1.0f           
	);
	
	if (m_target == nullptr)
	{
		SearchNearestPlayer();
	}
	
	if (m_target == nullptr)
	{
		return false;
	}
	
	bool hasOverlap;
	
	if (FVector::Dist(m_owner->GetActorLocation(),m_target->GetActorLocation()) < m_detectionRadius)
	{
		hasOverlap = true;
	}
	else
	{
		hasOverlap = false;
	}

	if (m_checkForInRange)
	{
		if (!hasOverlap) return false;
		else if (Cast<AEnemyCharacter>(m_owner)->IsActivated())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (!hasOverlap)
		{
			Cast<AEnemyCharacter>(m_owner)->SetCurrentTarget(nullptr);
			m_target = nullptr;
			return true;
		}
		else
		{
			return false;
		}
	}
}

void UFSM_ConditionPlayerInRange::SearchNearestPlayer()
{
	if (m_owner == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("In Range Cond AActor not found!"))
		return;
	}
	TArray<FOverlapResult> overlaps;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(m_owner);
	
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	
	bool hasOverlap = m_owner->GetWorld()->OverlapMultiByObjectType(
		overlaps,
		m_owner->GetActorLocation(),
		FQuat::Identity,
		objectQueryParams,
		FCollisionShape::MakeSphere(m_detectionRadius),
		queryParams
	);
	
	if (!hasOverlap) return;
	
	float distance = 0.0f;
	
	for (FOverlapResult& overlap : overlaps)
	{
		AActor* actor = overlap.GetActor();
		if (distance <= 0.01f && distance >= -0.01f)
		{
			distance = FVector::Dist(m_owner->GetActorLocation(),actor->GetActorLocation());
			m_target = Cast<ACharacter>(actor);
		}
		else if (FVector::Dist(m_owner->GetActorLocation(),actor->GetActorLocation()) < distance)
		{
			m_target = Cast<ACharacter>(actor);
		}
	}
	SetTarget(m_target);
}

void UFSM_ConditionPlayerInRange::SetTarget(ACharacter* a_target)
{
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_owner);
	enemy->SetCurrentTarget(a_target);
}
