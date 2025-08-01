// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "FSM_EnemyStateMachineComponent.h"
#include "Game_GameInstance.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "CustomChunkSystem/CustomChunkManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_skeletalMesh = GetMesh();

	m_weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	
	m_ownActionSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Own Action Sounds"));
	m_receivingActionSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Receiving Action Sounds"));

	m_stateMachine = CreateDefaultSubobject<UFSM_EnemyStateMachineComponent>(TEXT("StateMachineComponent"));

	m_characterHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("CharacterHitbox"));
	m_characterHitbox->SetupAttachment(m_skeletalMesh);
	m_characterHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_characterHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	m_characterHitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_characterHitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	m_weaponHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeHitbox"));
	m_weaponHitbox->SetupAttachment(m_weaponMesh);
	m_weaponHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_weaponHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	m_weaponHitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_weaponHitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	m_healthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	m_healthBarComponent->SetupAttachment(RootComponent);
	m_healthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);


	static ConstructorHelpers::FObjectFinder<USoundWave> soundAsset(TEXT("/Game/Enemy/SFX/SFX_EnemyHit.SFX_EnemyHit"));
	if (soundAsset.Succeeded())
	{
		m_hitSound = soundAsset.Object;
	}

}

void AEnemyCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FName boneName = FName("hand_r");

	if (m_weaponMesh)
	{
		m_weaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, boneName);
		m_weaponMesh->SetRelativeRotation(m_weaponRotation);
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_gameInstance = Cast<UGame_GameInstance>(GetGameInstance());

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	m_skeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	if (m_weaponMesh)
	m_weaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	m_characterHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHit);
	m_weaponHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHit);

	m_characterHitbox->UpdateOverlaps();
	m_weaponHitbox->UpdateOverlaps();

	m_currentHealth = m_maxHealth;

	m_weaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UUserWidget* widgetObject = m_healthBarComponent->GetUserWidgetObject();
	m_widgetHealthBar = Cast<UWidget_EnemyHealthBar>(widgetObject);

	m_ownActionSoundComp->SetVolumeMultiplier(m_gameInstance->GetSFXVolume());
	m_receivingActionSoundComp->SetVolumeMultiplier(m_gameInstance->GetSFXVolume());

	m_gameInstance->OnSFXVolumeChanged.AddDynamic(this, &AEnemyCharacter::HandleVolumeChanged);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::UpdateHealthBar()
{
	float healthPercent = m_currentHealth / m_maxHealth;
	m_widgetHealthBar->SetHealthPercent(healthPercent);
}


float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	m_currentHealth = FMath::Clamp(m_currentHealth - DamageAmount, 0.0f, m_maxHealth);

	UpdateHealthBar();

	m_receivingActionSoundComp->Sound = m_hitSound;
	m_receivingActionSoundComp->Play(0.0f);

	if (m_currentHealth <= 0.0f && !m_isDead)
	{
		OnDeath();
	}

	return DamageAmount;
}

void AEnemyCharacter::OnDeath()
{
	m_isDead = true;

	if (m_gameInstance && m_gameInstance->m_playerSave)
	{
		m_gameInstance->m_playerSave->m_currency += m_moneyValueOnDeath;
		m_gameInstance->GetPlayerUI()->UpdateShownMoney();
		UGameplayStatics::SaveGameToSlot(m_gameInstance->m_playerSave, TEXT("PlayerSaveSlot"), 0);
	}

	if (m_expOrbClass)
	{
		ACustomChunkManager* chunkManager = Cast<ACustomChunkManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomChunkManager::StaticClass()));
		AExpOrb* expOrb = Cast<AExpOrb>(chunkManager->SpawnActorInChunk(m_expOrbClass,GetActorLocation(),FRotator::ZeroRotator,FActorSpawnParameters()));
		if (expOrb)
		{
			expOrb->SetExpRange(m_minExpWorth, m_maxExpWorth);
		}
	}
	

	SetActorEnableCollision(false);
	GetCharacterMovement()->GravityScale = 0.1f;
	m_widgetHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	SetLifeSpan(4);
}

void AEnemyCharacter::TakeKnockback(float a_knockbackStrength, FVector a_knockbackDirection)
{
	HandleKnockback(a_knockbackDirection, a_knockbackStrength);
}


void AEnemyCharacter::HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength)
{
	LaunchCharacter(a_knockbackDirection * a_knockbackStrength,true,true);
}

void AEnemyCharacter::OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	if (a_otherActor && a_otherActor != this && a_otherComp)
	{
		if (a_otherComp->GetCollisionObjectType() == ECC_GameTraceChannel1)
		{
			//this->m_weaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UGameplayStatics::ApplyDamage(a_otherActor, m_attackDamage, GetController(), this, nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Enemy hit a player"))
		}
	}
}


void AEnemyCharacter::HandleVolumeChanged(float a_newVolume)
{
	if (m_ownActionSoundComp)
	{
		m_ownActionSoundComp->SetVolumeMultiplier(a_newVolume);
	}
	if (m_receivingActionSoundComp)
	{
		m_receivingActionSoundComp->SetVolumeMultiplier(a_newVolume);
	}
}
