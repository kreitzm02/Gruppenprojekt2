// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_GolemShockwave.h"

#include "Game_GameInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy_GolemShockwave::AEnemy_GolemShockwave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    m_hitbox->SetSphereRadius(0.f);
    m_hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_hitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_hitbox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    m_hitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_GolemShockwave::OnHit);
    RootComponent = m_hitbox;
    
    m_shockwaveVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShockwaveEffect"));
    m_shockwaveVFX->SetupAttachment(RootComponent);

	m_soundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Own Sounds"));
}

// Called when the game starts or when spawned
void AEnemy_GolemShockwave::BeginPlay()
{
	Super::BeginPlay();

	if (m_shockwaveVFX)
	{
		m_shockwaveVFX->Activate();
	}

	UGame_GameInstance* gameInstance = Cast<UGame_GameInstance>(GetGameInstance());

	m_soundComp->SetVolumeMultiplier(gameInstance->GetSFXVolume());

	gameInstance->OnSFXVolumeChanged.AddDynamic(this, &AEnemy_GolemShockwave::HandleVolumeChanged);

	m_shockwaveStartSound->bLooping = false;
	m_soundComp->Sound = m_shockwaveStartSound;
	m_soundComp->Play();
}

// Called every frame
void AEnemy_GolemShockwave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_currentRadius += m_expansionSpeed * DeltaTime;
	m_hitbox->SetSphereRadius(m_currentRadius);

	if (m_shockwaveVFX)
	{
		m_shockwaveVFX->SetFloatParameter(TEXT("Radius"), m_currentRadius);
	}

	if (m_currentRadius >= m_maxRadius)
	{
		Destroy();
	}

	if (m_enableDebug)
	DrawDebugSphere(
		this->GetWorld(),
		this->GetActorLocation(),
		m_currentRadius,
		16,
		FColor::Red,
		false,
		0.1f,
		0,
		1.0f
	);
}

void AEnemy_GolemShockwave::OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	if (a_otherActor && a_otherActor != this && a_otherComp)
	{
		if (a_otherComp->GetCollisionObjectType() == ECC_GameTraceChannel1)
		{
			UE_LOG(LogTemp,Warning,TEXT("projectile hit player"));
			UGameplayStatics::ApplyDamage(a_otherActor, m_enemyCharacter->GetAttackDamage(), m_enemyCharacter->GetController(), this, nullptr);
			
		}
	}
}

void AEnemy_GolemShockwave::HandleVolumeChanged(float a_newVolume)
{
	if (m_soundComp)
	{
		m_soundComp->SetVolumeMultiplier(a_newVolume);
	}
}