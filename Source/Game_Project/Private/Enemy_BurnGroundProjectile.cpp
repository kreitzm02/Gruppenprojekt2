// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_BurnGroundProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy_BurnGroundProjectile::AEnemy_BurnGroundProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	m_niagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Visuals"));
	m_niagaraComponent->SetupAttachment(RootComponent);
	m_hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hitbox"));
	m_hitbox->SetupAttachment(RootComponent);
	//m_hitbox->SetCollisionObjectType(ECC_Pawn);
	m_hitbox->SetCollisionResponseToAllChannels(ECR_Overlap);
	m_hitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

// Called when the game starts or when spawned
void AEnemy_BurnGroundProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_projectileStartPos = GetActorLocation();
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//m_niagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Portal VFX"));
	//m_niagaraComponent->SetAsset(m_projectileFlightNiagara);
	
	m_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		m_projectileFlightNiagara,
		GetActorLocation(),
		GetActorRotation(),
		FVector(1.0f),
		true,
		true
	);

	m_hitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

// Called every frame
void AEnemy_BurnGroundProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_ellapsedTime += DeltaTime;
	m_passedTimeLastDmgInterval += DeltaTime;
	
	if (!m_burnGroundCreated)
	{
		float alpha = m_ellapsedTime / m_flightTime;
	
		alpha = FMath::Clamp(alpha, 0.0f, 1.0f);
	
		if (alpha >= 1.0f)
		{
			//create burning ground
			m_hitbox->SetSphereRadius(m_hitboxSize);
			SetActorLocation(m_projectileTargetPos);
			m_hitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_BurnGroundProjectile::OnOverlapBegin);
			m_hitbox->OnComponentEndOverlap.AddDynamic(this, &AEnemy_BurnGroundProjectile::OnOverlapEnd);

			TArray<AActor*> overlappingActors;
			m_hitbox->GetOverlappingActors(overlappingActors, ACharacter::StaticClass());
			for (AActor* actor : overlappingActors)
			{
				if (Cast<ACharacter>(actor)->GetCapsuleComponent()->GetCollisionObjectType() == ECC_GameTraceChannel1)
				{
					m_overlappedActor = actor;
					GetWorld()->GetTimerManager().SetTimer(m_triggerTimerHandle, this, &AEnemy_BurnGroundProjectile::OnHit, m_possibleDamageIntervalInSec, true, 0.0f);
					break;
				}
			}

			m_niagaraComponent->DestroyComponent();
			m_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				m_burnGroundNiagara,
				GetActorLocation(),
				GetActorRotation(),
				FVector(1.0f),
				true,
				true
			);
			SetActorScale3D(m_burnGroundScale);
			m_ellapsedTime = 0.0f;
			m_burnGroundCreated = true;
		}
		else
		{
			FVector position = FMath::Lerp(m_projectileStartPos, m_projectileTargetPos, alpha);
	
			position.Z += FMath::Sin(alpha * PI) * m_flyingCurveHeight;
	
			SetActorLocation(position);
			m_niagaraComponent->SetRelativeLocation(position);
		}
	}
	else
	{
		if (m_enableDebug)
			DrawDebugSphere(
				this->GetWorld(),
				this->GetActorLocation(),
				m_hitboxSize,
				16,
				FColor::Red,
				false,
				0.1f,
				0,
				1.0f
			);
		
	}
	
	if (m_burnGroundCreated && m_ellapsedTime >= m_lifetime)
	{
		m_niagaraComponent->DestroyComponent();
		Destroy();
	}
}


void AEnemy_BurnGroundProjectile::OnOverlapBegin(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	if (a_otherActor != this && a_otherComp)
	{
		if (a_otherComp->GetCollisionObjectType() == ECC_GameTraceChannel1)
		{
			m_overlappedActor = a_otherActor;
			GetWorld()->GetTimerManager().SetTimer(m_triggerTimerHandle, this, &AEnemy_BurnGroundProjectile::OnHit, m_possibleDamageIntervalInSec, true, 0.0f);
		}
	}
}

void AEnemy_BurnGroundProjectile::OnOverlapEnd(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex)
{
	if (a_otherActor != this && a_otherComp)
	{
		if (a_otherComp->GetCollisionObjectType() == ECC_GameTraceChannel1)
		{
			GetWorld()->GetTimerManager().ClearTimer(m_triggerTimerHandle);
		}
	}
}


void AEnemy_BurnGroundProjectile::OnHit()
{
	
	if (m_passedTimeLastDmgInterval >= m_possibleDamageIntervalInSec)
	{
		UGameplayStatics::ApplyDamage(m_overlappedActor, m_owner->GetAttackDamage(), m_owner->GetController(), this, nullptr);
		m_passedTimeLastDmgInterval = 0.0f;
		UE_LOG(LogTemp,Error,TEXT("player hit by burning ground"))
	}

}

