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

	skeletalMesh = GetMesh();

	stateMachine = CreateDefaultSubobject<UFSM_EnemyStateMachineComponent>(TEXT("StateMachineComponent"));

	hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	hitbox->SetupAttachment(skeletalMesh);
	hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	hitbox->SetCollisionObjectType(ECC_WorldDynamic);
	hitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	hitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	hitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHit);

	currentHealth = maxHealth;
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
	currentHealth -= DamageAmount;

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
		UGameplayStatics::ApplyDamage(a_otherActor, attackDamage, GetController(), this, nullptr);
	}
}