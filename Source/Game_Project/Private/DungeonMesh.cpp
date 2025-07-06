// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMesh.h"

// Sets default values


ADungeonMesh::ADungeonMesh()
{
	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = m_MeshComp;
	m_MeshComp->SetStaticMesh(m_DefaultMesh);
}

// Called when the game starts or when spawned
void ADungeonMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

