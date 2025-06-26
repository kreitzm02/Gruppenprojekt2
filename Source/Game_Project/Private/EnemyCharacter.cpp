// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "FSM_EnemyStateMachineComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_skeletalMesh = GetMesh();

	m_weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	


	m_stateMachine = CreateDefaultSubobject<UFSM_EnemyStateMachineComponent>(TEXT("StateMachineComponent"));

	m_characterHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("CharacterHitbox"));
	m_characterHitbox->SetupAttachment(m_skeletalMesh);
	m_characterHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_characterHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	m_characterHitbox->SetCollisionResponseToAllChannels(ECR_Block);
	m_characterHitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	m_weaponHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeHitbox"));
	m_weaponHitbox->SetupAttachment(m_weaponMesh);
	m_weaponHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_weaponHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	m_weaponHitbox->SetCollisionResponseToAllChannels(ECR_Block);
	m_weaponHitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
}

void AEnemyCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FName boneName = FName("hand_r");

	m_weaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, boneName);
	m_weaponMesh->SetRelativeRotation(m_weaponRotation);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_characterHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHit);
	m_weaponHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHit);

	m_currentHealth = m_maxHealth;

	m_weaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	m_currentHealth -= DamageAmount;

	if (DamageCauser)
	{
		FVector knockbackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		knockbackDirection.Z = 0;
		knockbackDirection.Normalize();
		HandleKnockback(knockbackDirection,600.0f /*get knockback strengh from damage causer*/);
		//ACharacter* damagingUnit = Cast<ACharacter>(DamageCauser);
		//damagingUnit->GetKnockback();
	}

	return DamageAmount;
}

void AEnemyCharacter::HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength)
{
	LaunchCharacter(a_knockbackDirection * a_knockbackStrength,true,true);
}

void AEnemyCharacter::OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	if (a_otherActor && a_otherActor != this && a_otherComp)
	{
		//if (a_otherComp->GetCollisionObjectType() == ECC_GameTraceChannel1)
		//{
			UGameplayStatics::ApplyDamage(a_otherActor, m_attackDamage, GetController(), this, nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Enemy hit a player"))
		//}
	}
}