// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHubPortal.h"
#include <Kismet/GameplayStatics.h>
#include "LoadingScreenManager.h"
#include <Player/PlayerCharacter.h>

#include "Gamemode_Standart.h"
#include "Game_GameInstance.h"

// Sets default values
AMainHubPortal::AMainHubPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Root Component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Trigger Box
    m_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    m_BoxCollider->SetupAttachment(RootComponent);
    m_BoxCollider->SetBoxExtent(FVector(50.0f, 10.0f, 200.0f));
    m_BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_BoxCollider->SetCollisionObjectType(ECC_WorldDynamic);
    m_BoxCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_BoxCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    m_BoxCollider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
    m_BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMainHubPortal::OnOverlapBegin);

    // Visual Mesh (optional)
    m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_Mesh->SetupAttachment(RootComponent);
    m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMainHubPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainHubPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainHubPortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        ULoadingScreenManager::Get(Player->GetWorld())->StartLoading(Player->GetWorld());

    	//UGameplayStatics::OpenLevel(this, "temp");

        //AGamemode_Standart* gm = GetWorld()->GetAuthGameMode<AGamemode_Standart>();
        //gm->LoadNewMap("/Game/temp?listen");

        if (HasAuthority())
        {
	        DoServerTravel();
        }
        else
        {
	        RequestServerTravel(Cast<APlayerController>(Player->GetController()));
        }

        //bool temp;
        //NewLevel = ULevelStreamingDynamic::LoadLevelInstance(this, "temp", FVector::ZeroVector, FRotator::ZeroRotator,  temp);
        //NewLevel->SetShouldBeVisible(false);
        //NewLevel->SetShouldBeLoaded(true);
        //
        //FTimerHandle temp2;
        //
        //GetWorld()->GetTimerManager().SetTimer(temp2 , this, &AMainHubPortal::Load, 2.0f, false);
        

        UGame_GameInstance* gameInstance = Cast<UGame_GameInstance>(GetGameInstance());
        gameInstance->StartGameTimer();
        gameInstance->SetIsInLevel(true);
    }
}

void AMainHubPortal::RequestServerTravel_Implementation(APlayerController* a_pc)
{
	DoServerTravel();
}

void AMainHubPortal::DoServerTravel()
{
	if (!HasAuthority())
	{
		return;
	}
    if (m_travelInProgress)
    {
	    return;
    }
    m_travelInProgress = true;
     if (UWorld* world = GetWorld())
     {
         const ENetMode NetMode = GetNetMode();
         UE_LOG(LogTemp, Warning, TEXT("TRAVEL: NetMode=%d (0=Standalone,1=Dedicated,2=Listen,3=Client) HasAuthority=%d"),
             (int32)NetMode, HasAuthority() ? 1 : 0);
	     world->ServerTravel("/Game/temp?listen");
     }
}


void AMainHubPortal::Load()
{
    NewLevel->SetShouldBeVisible(true);
}
