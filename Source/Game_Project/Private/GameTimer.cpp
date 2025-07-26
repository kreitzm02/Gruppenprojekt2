// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimer.h"

#include "Blueprint/UserWidget.h"

// Sets default values
AGameTimer::AGameTimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameTimer::BeginPlay()
{
	Super::BeginPlay();

	m_remainingTime = m_maxGameTime;

	GetWorld()->GetTimerManager().SetTimer(m_levelTimerHandle, this, &AGameTimer::OnTimerFinished, m_maxGameTime, false);

	if (m_timerWidget)
	{
		m_timerWidgetInstance = CreateWidget<UWidget_GameTimer>(GetWorld(), m_timerWidget);
		if (m_timerWidgetInstance)
		{
			m_timerWidgetInstance->AddToViewport();
		}
	}
}

// Called every frame
void AGameTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_remainingTime -= DeltaTime;
	//update ui
	int remainingTime = FMath::CeilToInt(m_remainingTime);
	m_timerWidgetInstance->UpdateGameTime(remainingTime);
	
}

void AGameTimer::OnTimerFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Timer Ended"))
	//teleport to boss area


	//destroy widget and this
	m_timerWidgetInstance->RemoveFromParent();
	m_timerWidgetInstance->Destruct();
	Destroy();
}