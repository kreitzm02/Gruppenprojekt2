// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonGen.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADungeonGen::ADungeonGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_DevCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DevCube"));
	RootComponent = m_DevCube;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(*DEFAULT_CUBE_PATH);

	if (cubeMesh.Succeeded()) m_DevCube->SetStaticMesh(cubeMesh.Object);
}

// Called when the game starts or when spawned
void ADungeonGen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

