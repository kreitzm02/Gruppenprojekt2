// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonEntrancePortal.h"

// Sets default values
ADungeonEntrancePortal::ADungeonEntrancePortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	m_portalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Portal Trigger"));
	m_portalVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Portal VFX"));

	m_portalVFX->SetupAttachment(RootComponent);
	m_portalTrigger->SetupAttachment(m_portalVFX);

	m_portalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_portalTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	m_portalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_portalTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

// Called when the game starts or when spawned
void ADungeonEntrancePortal::BeginPlay()
{
	Super::BeginPlay();

	m_portalTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADungeonEntrancePortal::OnPortalEnter);
}

// Called every frame
void ADungeonEntrancePortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//enemies spawn logic
}

void ADungeonEntrancePortal::OnPortalEnter(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Entered Portal"))
}

void ADungeonEntrancePortal::SetDungeonAsCleared()
{
	m_dungeonCleared = true;
	SetPortalClearColor();
}

