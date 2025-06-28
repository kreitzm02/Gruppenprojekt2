// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_MageProjectile.h"

#include "Components/SphereComponent.h"

AEnemy_MageProjectile::AEnemy_MageProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	m_projectileHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	m_projectileHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_projectileHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	m_projectileHitbox->SetCollisionResponseToAllChannels(ECR_Block);
	m_projectileHitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	RootComponent = m_projectileHitbox;

	m_projectileMesh->SetupAttachment(RootComponent);


}

void AEnemy_MageProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_MageProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveInDirection(DeltaTime);
}

void AEnemy_MageProjectile::MoveInDirection(float a_deltaTime)
{
	Super::MoveInDirection(a_deltaTime);
}