// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_ProjectileBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy_ProjectileBase::AEnemy_ProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));

	m_projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
}

void AEnemy_ProjectileBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_projectileMesh->SetRelativeRotation(m_arrowRotation);
	m_projectileHitbox->SetRelativeScale3D(m_hitboxSize);
}

// Called when the game starts or when spawned
void AEnemy_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("arrow beginplay"))

	SetLifeSpan(m_lifeSpan);
	m_projectileHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_ProjectileBase::OnHit);

	m_targetDirection = m_target->GetActorLocation() - this->GetActorLocation();
	m_targetDirection.Normalize();
	this->SetActorRotation(m_targetDirection.Rotation());
}

// Called every frame
void AEnemy_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy_ProjectileBase::MoveInDirection(float a_deltaTime)
{
	SetActorLocation(GetActorLocation() + m_targetDirection * m_projectileSpeed * a_deltaTime);
}

void AEnemy_ProjectileBase::OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("projectile hit something"));
	if (a_otherActor && a_otherActor != this && a_otherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("projectile hit something"));
		if (a_otherComp->GetCollisionObjectType() == ECC_GameTraceChannel1)
		{
			UE_LOG(LogTemp,Warning,TEXT("projectile hit player"));
			UGameplayStatics::ApplyDamage(a_otherActor, m_enemyCharacter->GetAttackDamage(), m_enemyCharacter->GetController(), m_enemyCharacter, nullptr);
			this->Destroy();
		}
		else
		{
			//Destroy();
		}
	}
}
