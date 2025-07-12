// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM_RiseFromGround.h"
#include "Enemy_NecroSummon.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFSM_RiseFromGround::Initialize()
{
	Super::Initialize();
	m_thisEnemy = Cast<AEnemy_NecroSummon>(m_ownerCharacter);
	m_riseAnimation = m_thisEnemy->GetRiseAnimation();
	m_duration = m_riseAnimation->GetPlayLength();
	m_duration = m_duration/2;
}

void UFSM_RiseFromGround::OnEnter()
{
	Super::OnEnter();
	m_ownerSkeletalMesh->PlayAnimation(m_riseAnimation, false);
	FVector temp = m_thisEnemy->GetActorLocation();
	startVec = temp;
	endVec = temp;
	endVec.Z += m_thisEnemy->GetHeightOffset() * -1;
}

void UFSM_RiseFromGround::OnUpdate(float a_deltatime)
{
	Super::OnUpdate(a_deltatime);

	m_passedTime += a_deltatime;
	float alpha = FMath::Clamp(m_passedTime / m_duration, 0.0f, 1.0f);
	FVector newPos = FMath::Lerp(startVec, endVec, alpha);
	m_thisEnemy->SetActorLocation(newPos);
}

void UFSM_RiseFromGround::OnExit()
{
	Super::OnExit();

	AEnemy_NecroSummon* enemy = Cast<AEnemy_NecroSummon>(m_ownerCharacter);
	enemy->GetCharacterMovement()->GravityScale = 1.0f;
	enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	enemy->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}