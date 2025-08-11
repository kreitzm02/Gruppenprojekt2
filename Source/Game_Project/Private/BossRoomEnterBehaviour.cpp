// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomEnterBehaviour.h"
#include <Player/PlayerCharacter.h>
#include "Engine/StaticMeshActor.h"
#include "Components/AudioComponent.h"
#include "DungeonDoorBehaviour.h"
#include "BossEnemy_Golem.h"
#include "BossEnemy_Mage.h"
#include "BossEnemy_Rogue.h"
#include "BossEnemy_Warrior.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossRoomEnterBehaviour::ABossRoomEnterBehaviour()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	m_TriggerBox->SetupAttachment(RootComponent);
	m_TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	m_TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	m_TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossRoomEnterBehaviour::OnTriggerOverlap);

	
}

// Called when the game starts or when spawned
void ABossRoomEnterBehaviour::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADungeonDoorBehaviour::StaticClass(), m_DungeonDoors);

	TArray<AActor*> tempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossEnemy_Golem::StaticClass(), tempActors);
	m_BossEnemies.Append(tempActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossEnemy_Mage::StaticClass(), tempActors);
	m_BossEnemies.Append(tempActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossEnemy_Warrior::StaticClass(), tempActors);
	m_BossEnemies.Append(tempActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossEnemy_Rogue::StaticClass(), tempActors);
	m_BossEnemies.Append(tempActors);


	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MusicTag"), foundActors);

	if (foundActors.Num() > 0)
	{
		m_DungeonMusicContainer = foundActors[0];
	}
}

// Called every frame
void ABossRoomEnterBehaviour::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossRoomEnterBehaviour::OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor) && !m_Overlapped)
	{
		m_Overlapped = true;
		if (m_DungeonMusicContainer)
		{
			if (UAudioComponent* audioComp = m_DungeonMusicContainer->FindComponentByClass<UAudioComponent>())
			{
				audioComp->SetSound(Cast<USoundBase>(m_BossMusic));
				audioComp->Play();
			}
		}

		for (AActor* a : m_DungeonDoors)
		{
			if (ADungeonDoorBehaviour* door = Cast<ADungeonDoorBehaviour>(a))
			{
				door->m_ShouldOpen = true;
			}
		}

		for (AActor* a : m_BossEnemies)
		{
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(a))
			{
				enemy->Activate();
			}
		}
	}
}

