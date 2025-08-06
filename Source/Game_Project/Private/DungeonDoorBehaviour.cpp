// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonDoorBehaviour.h"
#include <Player/PlayerCharacter.h>

// Sets default values
ADungeonDoorBehaviour::ADungeonDoorBehaviour()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = m_DoorMesh;

    m_TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    m_TriggerBox->SetupAttachment(RootComponent);
    m_TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));
    m_TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
    m_TriggerBox->SetRelativeLocation(FVector(0.f, 0.f, -100.f)); // Auf Bodenniveau vor der Tür

    m_TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADungeonDoorBehaviour::OnTriggerOverlap);
}

// Called when the game starts or when spawned
void ADungeonDoorBehaviour::BeginPlay()
{
	Super::BeginPlay();
    m_ClosedPosition = GetActorLocation();
}

// Called every frame
void ADungeonDoorBehaviour::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (m_ShouldOpen)
    {
        FVector current = GetActorLocation();
        FVector newLocation = FMath::VInterpConstantTo(current, m_ClosedPosition + FVector(0.f, 0.f, 400.f), DeltaTime, m_MoveSpeed);
        SetActorLocation(newLocation);
    }
}

void ADungeonDoorBehaviour::OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
    {
		
	}
}

