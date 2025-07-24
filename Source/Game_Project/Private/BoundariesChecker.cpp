// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundariesChecker.h"

#include "BossEnemy_Endboss.h"

// Sets default values
ABoundariesChecker::ABoundariesChecker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoundariesChecker::BeginPlay()
{
	Super::BeginPlay();
	m_moveDirection = FVector(1,0,0);
}

// Called every frame
void ABoundariesChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + m_moveDirection * m_moveSpeed);

	FHitResult hitResult;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + (m_moveDirection * m_lineTraceLength);

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);
	traceParams.AddIgnoredActor(m_owner);
	traceParams.AddIgnoredActor(m_player);

	bool hit = GetWorld()->LineTraceSingleByChannel(
		hitResult,
		start,
		end,
		ECC_Visibility,
		traceParams
	);

	if (m_enableDebug)
	{
		DrawDebugLine(
			GetWorld(),
			start,
			end,
			hit ? FColor::Red : FColor::Green,
			false,
			0.1f,
			0,
			1.0f
		);
	}

	if (hit)
	{
		m_moveDirection = FRotator(0, 90, 0).RotateVector(m_moveDirection);
		m_boundariesChecked++;
		switch (m_boundariesChecked)
		{
		case 1:
			m_xSizeMax = GetActorLocation().X;
			UE_LOG(LogTemp, Warning, TEXT("X Max: %f"), m_xSizeMax)
			break;
		case 2:
			m_ySizeMax = GetActorLocation().Y;
			UE_LOG(LogTemp, Warning, TEXT("Y Max: %f"), m_ySizeMax)
			break;
		case 3:
			m_xSizeMin = GetActorLocation().X;
			UE_LOG(LogTemp, Warning, TEXT("X Min: %f"), m_xSizeMin)
			break;
		case 4:
			m_ySizeMin = GetActorLocation().Y;
			UE_LOG(LogTemp, Warning, TEXT("Y Min: %f"), m_ySizeMin)
			break;
		case 5:
		{
			ABossEnemy_Endboss* enemy = Cast<ABossEnemy_Endboss>(m_owner);
			if (enemy)
			{
				enemy->SetArenaXMax(m_xSizeMax);
				enemy->SetArenaYMax(m_ySizeMax);
				enemy->SetArenaXMin(m_xSizeMin);
				enemy->SetArenaYMin(m_ySizeMin);
				enemy->SetBoundariesChecked(true);
			}
			
			Destroy();
			break;
		}
		default:
			break;
		}
	}

}

