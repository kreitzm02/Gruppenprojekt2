// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_CondPlayerOutAtkRange.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "EnemyCharacter.h"
#include "Engine/OverlapResult.h"


void UFSM_CondPlayerOutAtkRange::Initialize()
{
	m_owner = Cast<AActor>(GetOuter()->GetOuter());

	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_owner);
	m_attackRange = enemy->GetAttackRange() + 100;
	m_attackDuration = enemy->GetAttackDuration();
}

void UFSM_CondPlayerOutAtkRange::ResetCondition()
{
	m_passedTime = 0.0f;
	
	m_target = nullptr;
	
	if (Cast<AEnemyCharacter>(m_owner)->GetCurrentTarget() != nullptr)
	{
		m_target = Cast<AEnemyCharacter>(m_owner)->GetCurrentTarget();
	}
}

bool UFSM_CondPlayerOutAtkRange::IsConditionMet(float a_deltaTime)
{
	m_passedTime += a_deltaTime;

	if (m_owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("In Range Cond AActor not found!"))
			return false;
	}
	
	if (m_target == nullptr)
	{
		if (Cast<AEnemyCharacter>(m_owner)->GetCurrentTarget() != nullptr)
		{
			m_target = Cast<AEnemyCharacter>(m_owner)->GetCurrentTarget();
		}
		else
		{
			return false;
		}
	}
	
	//TArray<FOverlapResult> overlaps;
	//FCollisionQueryParams queryParams;
	//queryParams.AddIgnoredActor(m_owner);

	//FCollisionObjectQueryParams objectQueryParams;
	//objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);

	//bool hasOverlap = m_owner->GetWorld()->OverlapMultiByObjectType(
	//	overlaps,
	//	m_owner->GetActorLocation(),
	//	FQuat::Identity,
	//	objectQueryParams,
	//	FCollisionShape::MakeSphere(m_attackRange),
	//	queryParams
	//);
	
	if (m_enableDebug)
	DrawDebugSphere(
		m_owner->GetWorld(),
		m_owner->GetActorLocation(),
		m_attackRange,
		16,
		FColor::Red,
		false,
		0.1f,
		0,
		1.0f
	);

	bool hasOverlap = false;
	
	if (FVector::Dist(m_owner->GetActorLocation(),m_target->GetActorLocation()) <= m_attackRange)
	{
		hasOverlap = true;
	}
	else
	{
		hasOverlap = false;
	}
	
	if (!hasOverlap && m_passedTime >= m_attackDuration)
	{
		return true;
	}
	else if (hasOverlap && m_passedTime >= m_attackDuration)
	{
		m_passedTime = 0.0f;


		if (!m_isDungeonEnemy)
		{
			return false;
		}
		else
		{
			FHitResult hitResult;
			FVector start = m_owner->GetActorLocation();
			FVector end = m_target->GetActorLocation();

			FCollisionQueryParams traceParams;
			traceParams.AddIgnoredActor(m_owner);
			traceParams.AddIgnoredActor(m_target);

			bool hit = m_owner->GetWorld()->LineTraceSingleByChannel(
				hitResult,
				start,
				end,
				ECC_Visibility,
				traceParams
			);

			if (m_enableDebug)
			{
				DrawDebugLine(
					m_owner->GetWorld(),
					start,
					end,
					hit ? FColor::Red : FColor::Green,
					false,
					0.1f,
					0,
					1.0f
				);
			}
			if (hit) return true;
				
			return false;
		}
	}

	return false;
}

//void UFSM_CondPlayerOutAtkRange::SearchNearestPlayer()
//{
//	if (m_owner == nullptr)
//	{
//		UE_LOG(LogTemp,Error,TEXT("In Range Cond AActor not found!"))
//		return;
//	}
//	TArray<FOverlapResult> overlaps;
//	FCollisionQueryParams queryParams;
//	queryParams.AddIgnoredActor(m_owner);
//	
//	FCollisionObjectQueryParams objectQueryParams;
//	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
//	
//	bool hasOverlap = m_owner->GetWorld()->OverlapMultiByObjectType(
//		overlaps,
//		m_owner->GetActorLocation(),
//		FQuat::Identity,
//		objectQueryParams,
//		FCollisionShape::MakeSphere(m_detectionRadius),
//		queryParams
//	);
//	
//	if (!hasOverlap) return;
//	
//	float distance = 0.0f;
//	
//	for (FOverlapResult& overlap : overlaps)
//	{
//		AActor* actor = overlap.GetActor();
//		if (distance <= 0.01f && distance >= -0.01f)
//		{
//			distance = FVector::Dist(m_owner->GetActorLocation(),actor->GetActorLocation());
//			m_target = Cast<ACharacter>(actor);
//		}
//		else if (FVector::Dist(m_owner->GetActorLocation(),actor->GetActorLocation()) < distance)
//		{
//			m_target = Cast<ACharacter>(actor);
//		}
//	}
//	SetTarget(m_target);
//}
//
//void UFSM_CondPlayerOutAtkRange::SetTarget(ACharacter* a_target)
//{
//	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(m_owner);
//	enemy->SetCurrentTarget(a_target);
//}