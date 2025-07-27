// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_GolemBoulder.h"

#include "Enemy_GolemShockwave.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy_GolemBoulder::AEnemy_GolemBoulder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_meshComponent;
	m_meshComponent->SetSimulatePhysics(true);
	m_meshComponent->SetEnableGravity(true);
	m_meshComponent->SetNotifyRigidBodyCollision(true);
	m_meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	m_underBoulderHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	m_underBoulderHitbox->SetupAttachment(RootComponent);
	m_underBoulderHitbox->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AEnemy_GolemBoulder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_underBoulderHitbox->SetRelativeScale3D(m_hitboxScale);
}

// Called when the game starts or when spawned
void AEnemy_GolemBoulder::BeginPlay()
{
	Super::BeginPlay();
	m_underBoulderHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_GolemBoulder::OnHit);
	m_meshComponent->OnComponentHit.AddDynamic(this, &AEnemy_GolemBoulder::OnHitPhysically);
}

// Called every frame
void AEnemy_GolemBoulder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_GolemBoulder::OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	if (a_otherActor != this)
	{
		if (a_otherComp)
		{
			if (a_otherComp->GetCollisionObjectType() == ECC_GameTraceChannel1)
			{
				UGameplayStatics::ApplyDamage(a_otherActor, m_owner->GetAttackDamage(), m_owner->GetController(), this, nullptr);
				DestroyBoulder();
			}
		}
	}
}

void AEnemy_GolemBoulder::OnHitPhysically(UPrimitiveComponent* a_hitComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, FVector a_normalImpulse, const FHitResult& a_hit)
{
	if (a_otherActor && a_otherActor != this)
	{
		if (a_otherComp)
		{
			DestroyBoulderWithShockwave();
		}
		else
		{
			DestroyBoulderWithShockwave();
		}
	}
}


void AEnemy_GolemBoulder::DestroyBoulder()
{
	if (m_boulderDestroyNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			m_boulderDestroyNiagara,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.0f),
			true,
			true
		);
	}

	Destroy();
}

void AEnemy_GolemBoulder::DestroyBoulderWithShockwave()
{
	if (m_boulderDestroyNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			m_boulderDestroyNiagara,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.0f),
			true,
			true
		);
	}

	FTransform transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), GetActorLocation(), FVector(1.0f, 1.0f, 1.0f));

	AEnemy_GolemShockwave* shockwave = GetWorld()->SpawnActorDeferred<AEnemy_GolemShockwave>(m_shockwaveBP, transform);
	shockwave->SetOwnerEnemy(m_owner);
	shockwave->SetExpansionSpeed(m_shockwaveExpansionSpeed);
	shockwave->SetMaxRadius(m_shockwaveMaxRadius);
	shockwave->SetStartRadius(m_shockwaveStartRadius);
	UGameplayStatics::FinishSpawningActor(shockwave, transform);

	Destroy();
}
