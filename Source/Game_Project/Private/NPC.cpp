// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = m_MeshComp;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	if (m_MeshComp)
	{
		m_MeshComp->SetSkeletalMesh(m_NPCMesh);
		m_MeshComp->PlayAnimation(m_IdleAnimation, true);
	}
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ANPC::IsInteractedWith()
{
	return false;
}

void ANPC::PlayInteraction()
{
}

