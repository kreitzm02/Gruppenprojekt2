// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonExitPortal.h"
#include "DungeonLevelStreamer.h"
#include "Game_GameInstance.h"
#include "LoadingScreenManager.h"

// Sets default values
ADungeonExitPortal::ADungeonExitPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	m_portalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Portal Trigger"));
	m_portalVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Portal VFX"));
	m_DungeonLevelStreamer = CreateDefaultSubobject<UDungeonLevelStreamer>(TEXT("Dungeon Level Streaming Component"));

	m_portalVFX->SetupAttachment(RootComponent);
	m_portalTrigger->SetupAttachment(m_portalVFX);

	m_portalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_portalTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	m_portalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_portalTrigger->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

// Called when the game starts or when spawned
void ADungeonExitPortal::BeginPlay()
{
	Super::BeginPlay();

	m_portalTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADungeonExitPortal::OnPortalEnter);
}

// Called every frame
void ADungeonExitPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonExitPortal::OnPortalEnter(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered Portal"))
	ULoadingScreenManager::Get(GetWorld())->StartLoading(GetWorld());
	m_DungeonLevelStreamer->UnloadDungeon();
	a_otherActor->SetActorLocation(Cast<UGame_GameInstance>(GetGameInstance())->GetLastOverworldPos());
	ULoadingScreenManager::Get(GetWorld())->EndLoading();
	Destroy();
}

