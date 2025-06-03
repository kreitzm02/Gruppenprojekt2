// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Animation/AnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupCamera();
	SetupMovement();

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->SetSkeletalMesh(m_PlayerCharDataAssets[0]->m_Mesh);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void APlayerCharacter::MoveForward(float a_Value)
{
	if (Controller && (a_Value != 0.0f))
	{
		const FVector direction = FVector(1.0f, 0.0f, 0.0f);
		AddMovementInput(direction, a_Value);
	}
}

void APlayerCharacter::MoveRight(float a_Value)
{
	if (Controller && (a_Value != 0.0f))
	{
		const FVector direction = FVector(0.0f, 1.0f, 0.0f);
		AddMovementInput(direction, a_Value);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::SetupCamera()
{
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraBoom->SetupAttachment(RootComponent);
	//m_CameraBoom->TargetArmLength = 300.0f;
	//m_CameraBoom->bUsePawnControlRotation = false;
	//m_CameraBoom->bDoCollisionTest = false;
	//m_CameraBoom->bInheritYaw = false;
	//m_CameraBoom->bInheritPitch = false;
	//m_CameraBoom->bInheritRoll = false;

	m_PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	m_PlayerCamera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
	//m_PlayerCamera->bUsePawnControlRotation = false;
}

void APlayerCharacter::SetupMovement()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->AirControl = 0.0f;

}

