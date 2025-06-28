// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_RogueArrow.h"

#include "Components/BoxComponent.h"

AEnemy_RogueArrow::AEnemy_RogueArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	m_projectileHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	m_projectileHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_projectileHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	m_projectileHitbox->SetCollisionResponseToAllChannels(ECR_Block);
	m_projectileHitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	RootComponent = m_projectileHitbox;

	m_projectileMesh->SetupAttachment(RootComponent);

	
}

void AEnemy_RogueArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_RogueArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveInDirection(DeltaTime);
}

void AEnemy_RogueArrow::MoveInDirection(float a_deltaTime)
{
	Super::MoveInDirection(a_deltaTime);
}
