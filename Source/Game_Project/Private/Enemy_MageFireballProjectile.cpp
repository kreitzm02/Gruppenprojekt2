// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_MageFireballProjectile.h"
#include "Components/SphereComponent.h"

AEnemy_MageFireballProjectile::AEnemy_MageFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	m_projectileHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	m_projectileHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_projectileHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	m_projectileHitbox->SetCollisionResponseToAllChannels(ECR_Block);
	m_projectileHitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	//RootComponent = m_projectileHitbox;

	m_projectileMesh->SetupAttachment(RootComponent);
	m_projectileHitbox->SetupAttachment(m_projectileMesh);

}

void AEnemy_MageFireballProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_projectileHitbox->SetRelativeScale3D(m_hitboxBoxSize);

	Cast<USphereComponent>(m_projectileHitbox)->SetSphereRadius(m_hitboxSphereSize);

	//m_projectileHitbox->SetRelativeScale3D_Direct(m_hitboxBoxSize);
}

void AEnemy_MageFireballProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_MageFireballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_targetDirection = m_target->GetActorLocation() - this->GetActorLocation();
	m_targetDirection.Z = 0.0f;
	m_targetDirection.Normalize();
	this->SetActorRotation(m_targetDirection.Rotation());
	MoveInDirection(DeltaTime);
}

void AEnemy_MageFireballProjectile::MoveInDirection(float a_deltaTime)
{
	Super::MoveInDirection(a_deltaTime);
}