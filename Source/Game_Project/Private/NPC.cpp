// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NPCMesh"));
	RootComponent = m_Mesh;

	m_InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	m_InteractionWidget->SetupAttachment(RootComponent);
	
	m_InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	m_InteractionSphere->InitSphereRadius(200.f);
	m_InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_InteractionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	m_InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	m_InteractionSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	m_InteractionSphere->SetupAttachment(RootComponent);

	m_InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnSphereBeginOverlap);
	m_InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnSphereEndOverlap);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	if (m_IdleAnimation && m_Mesh)
	{
		m_Mesh->PlayAnimation(m_IdleAnimation, true);
	}

	GetWorld()->GetTimerManager().SetTimer(m_RandomAnimTimer, this, &ANPC::TryPlayRandomAnim, 5.0f, true);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact()
{
	
}

void ANPC::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		m_OverlappingPlayer = Player;
	}
}

void ANPC::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == m_OverlappingPlayer)
	{
		m_OverlappingPlayer = nullptr;
	}
}

void ANPC::TryPlayRandomAnim()
{
	//if (!m_Mesh || m_RandomAnims.Num() == 0) return;
	//
	//if (FMath::RandRange(0, 100) > 33) return;
	//
	//m_Mesh->Stop();
	//
	//int32 index = FMath::RandRange(0, m_RandomAnims.Num() - 1);
	//UAnimSequence* selectedAnim = m_RandomAnims[index];
	//
	//if (selectedAnim)
	//{
	//	m_Mesh->PlayAnimation(selectedAnim, false);
	//
	//	FTimerHandle resumeIdleHandle;
	//	float duration = selectedAnim->GetPlayLength();
	//
	//	GetWorld()->GetTimerManager().SetTimer(resumeIdleHandle, [this]()
	//		{
	//			if (m_IdleAnimation && m_Mesh)
	//			{
	//				m_Mesh->PlayAnimation(m_IdleAnimation, true);
	//			}
	//		}, duration, false);
	//}
}

