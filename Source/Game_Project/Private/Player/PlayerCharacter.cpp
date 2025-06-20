// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "AbilityComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupCamera();
	SetupMovement();
	SetupAbilityComp();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupPlayer();

	if (m_PlayerCharDataAssets[0]->m_IdleAnim)
		GetMesh()->PlayAnimation(m_PlayerCharDataAssets[0]->m_IdleAnim, true);

	m_PreviousLocation = GetActorLocation();

	SetupWeapons();
	HideAllWeapons();
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//currentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Player was hit"))
	if (DamageCauser)
	{
		FVector knockbackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		knockbackDirection.Z = 0;
		knockbackDirection.Normalize();
		HandleKnockback(knockbackDirection, 6000.0f /*get knockback strengh from damage causer*/);
		//ACharacter* damagingUnit = Cast<ACharacter>(DamageCauser);
		//damagingUnit->GetKnockback();
	}

	return DamageAmount;
}

void APlayerCharacter::HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength)
{
	LaunchCharacter(a_knockbackDirection * a_knockbackStrength, true, true);
}

void APlayerCharacter::OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	if (a_otherActor && a_otherActor != this && a_otherComp)
	{
		UGameplayStatics::ApplyDamage(a_otherActor, 1.0f/*get player damage*/, GetController(), this, nullptr);
	}
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

void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = m_RunSpeed;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;
}

void APlayerCharacter::UseAbility()
{
	m_PlayerAbilities->ActivateAbility(m_CurrentAbilitySlot); // TODO : 0 is temp only, param must be the index of the "active" ability (player can have multiple abilties)
}

void APlayerCharacter::ChangeToAbilitySlot0()
{
	m_CurrentAbilitySlot = 0;
	m_PlayerAbilities->EquipAbility(0);
}

void APlayerCharacter::ChangeToAbilitySlot1()
{
	m_CurrentAbilitySlot = 1;
	m_PlayerAbilities->EquipAbility(1);
}

void APlayerCharacter::ChangeToAbilitySlot2()
{
	m_CurrentAbilitySlot = 2;
	m_PlayerAbilities->EquipAbility(2);
}

void APlayerCharacter::ChangeToAbilitySlot3()
{
	m_CurrentAbilitySlot = 3;
	m_PlayerAbilities->EquipAbility(3);
}

void APlayerCharacter::AbilitySlotIncrease()
{
	if (m_CurrentAbilitySlot == m_AbilityNum - 1) m_CurrentAbilitySlot = 0;
	else if (m_CurrentAbilitySlot >= m_AbilityNum || m_CurrentAbilitySlot <= -1) m_CurrentAbilitySlot = 0;
	else m_CurrentAbilitySlot++;
	m_PlayerAbilities->EquipAbility(m_CurrentAbilitySlot);
}

void APlayerCharacter::AbilitySlotDecrease()
{
	if (m_CurrentAbilitySlot == 0) m_CurrentAbilitySlot = m_AbilityNum - 1;
	else if (m_CurrentAbilitySlot >= m_AbilityNum || m_CurrentAbilitySlot <= -1) m_CurrentAbilitySlot = 0;
	else m_CurrentAbilitySlot--;
	m_PlayerAbilities->EquipAbility(m_CurrentAbilitySlot);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector currentLocation = GetActorLocation();
	const float speed = FVector::Dist(currentLocation, m_PreviousLocation) / DeltaTime;
	m_PreviousLocation = currentLocation;
	const float movementThreshold = 5.0f;

	if (speed > movementThreshold)
	{
		if (speed > m_WalkSpeed + movementThreshold && m_PlayerState != EPlayerState::SPRINT)
		{
			m_PlayerState = EPlayerState::SPRINT;
			GetMesh()->PlayAnimation(m_PlayerCharDataAssets[0]->m_SprintAnim, true);
		}
		else if (speed < m_WalkSpeed + movementThreshold && m_PlayerState != EPlayerState::WALK)
		{
			m_PlayerState = EPlayerState::WALK;
			GetMesh()->PlayAnimation(m_PlayerCharDataAssets[0]->m_WalkAnim, true);
		}
		
	}
	else
	{
		if (m_PlayerState != EPlayerState::IDLE)
		{
			m_PlayerState = EPlayerState::IDLE;
			GetMesh()->PlayAnimation(m_PlayerCharDataAssets[0]->m_IdleAnim, true);
		}
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);
	PlayerInputComponent->BindAction("UseAbility", IE_Pressed, this, &APlayerCharacter::UseAbility);
	PlayerInputComponent->BindAction("EquipAbilityA", IE_Pressed, this, &APlayerCharacter::ChangeToAbilitySlot0);
	PlayerInputComponent->BindAction("EquipAbilityB", IE_Pressed, this, &APlayerCharacter::ChangeToAbilitySlot1);
	PlayerInputComponent->BindAction("EquipAbilityC", IE_Pressed, this, &APlayerCharacter::ChangeToAbilitySlot2);
	PlayerInputComponent->BindAction("EquipAbilityD", IE_Pressed, this, &APlayerCharacter::ChangeToAbilitySlot3);
	PlayerInputComponent->BindAction("AbilityIncrease", IE_Pressed, this, &APlayerCharacter::AbilitySlotIncrease);
	PlayerInputComponent->BindAction("AbilityDecrease", IE_Pressed, this, &APlayerCharacter::AbilitySlotDecrease);
}

void APlayerCharacter::SetWeaponVisibility(FName a_BoneName, bool a_SetVisible)
{
	if (UStaticMeshComponent** found = m_AttachedWeapons.Find(a_BoneName))
	{
		UStaticMeshComponent* component = *found;
		component->SetVisibility(a_SetVisible, true);
		component->SetHiddenInGame(!a_SetVisible, true);
		component->SetCollisionEnabled(a_SetVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1440.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->AirControl = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = m_WalkSpeed;

}

void APlayerCharacter::SetupAbilityComp()
{
	m_PlayerAbilities = CreateDefaultSubobject<UAbilityComponent>(TEXT("PlayerAbilities"));
}

void APlayerCharacter::SetupPlayer()
{
	GetMesh()->SetSkeletalMesh(m_PlayerCharDataAssets[0]->m_Mesh);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[0]->m_StartingAbility);
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[0]->m_StartingAbility1Debug);
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[0]->m_StartingAbility2Debug);
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[0]->m_StartingAbility3Debug);
}

void APlayerCharacter::SetupWeapons()
{
	USkeletalMeshComponent* skeletalMesh = GetMesh();
	if (!skeletalMesh) return;

	const FName slotBone = TEXT("handslot_r");
	if (skeletalMesh->GetBoneIndex(slotBone) == INDEX_NONE) return;

	TArray<FName> weaponBones;
	GetChildBones(slotBone, weaponBones);

	for (FName boneName : weaponBones)
	{
		UStaticMesh* meshAsset;
		if (boneName == TEXT("1H_Axe")) meshAsset = m_Axe;
		else if (boneName == TEXT("1H_Sword")) meshAsset = m_Sword;
		else if (boneName == TEXT("1H_Wand")) meshAsset = m_Wand;
		else if (boneName == TEXT("Knife")) meshAsset = m_Blade;
		else if (boneName == TEXT("1H_Crossbow")) meshAsset = m_Crossbow;
		else if (boneName == TEXT("2H_Mace")) meshAsset = m_Mace;
		else if (boneName == TEXT("2H_Staff")) meshAsset = m_Staff;
		else if (boneName == TEXT("1H_Dagger")) meshAsset = m_Dagger;
		else if (boneName == TEXT("1H_Scythe")) meshAsset = m_Scythe;
		else continue;

		UStaticMeshComponent* meshComponent = AttachWeaponComponentToBone(boneName, meshAsset);
		//if (meshComponent)
		//{
		//	meshComponent->SetVisibility(false, true);
		//	meshComponent->SetHiddenInGame(true, true);
		//	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//}
	}
}

void APlayerCharacter::GetChildBones(const FName& a_ParentBoneName, TArray<FName>& a_OutChildBones) const
{
	USkeletalMeshComponent* skeletalMesh = GetMesh();
	if (!skeletalMesh) return;

	USkeletalMesh* meshAsset = skeletalMesh->GetSkeletalMeshAsset();
	if (!meshAsset) return;

	const FReferenceSkeleton& refSkeleton = meshAsset->GetRefSkeleton();
	int32 parentIndex = refSkeleton.FindBoneIndex(a_ParentBoneName);
	if (parentIndex == INDEX_NONE) return;

	int32 bonesNum = refSkeleton.GetNum();
	for (int32 i = 0; i < bonesNum; i++)
	{
		if (i == parentIndex) continue;
		int32 currentIndex = i;

		while (currentIndex != INDEX_NONE)
		{
			currentIndex = refSkeleton.GetParentIndex(currentIndex);
			if (currentIndex == parentIndex)
			{
				a_OutChildBones.AddUnique(refSkeleton.GetBoneName(i));
				break;
			}
		}
	}
}

void APlayerCharacter::GetWeaponSockets(const FName& a_ParentBoneName, TArray<FName>& a_OutSockets) const
{
	a_OutSockets.Reset();

	USkeletalMeshComponent* skelComp = GetMesh();
	if (!skelComp || !skelComp->SkeletalMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWeaponSlotsBySocket: Kein valid SkeletalMeshComponent oder SkeletalMesh."));
		return;
	}

	USkeleton* Skeleton = skelComp->SkeletalMesh->GetSkeleton();
	if (!Skeleton)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWeaponSlotsBySocket: Kein Skeleton im Asset."));
		return;
	}

	// Schau alle Sockets durch, die auf genau diesem Bone sitzen
	for (USkeletalMeshSocket* socket : Skeleton->Sockets)
	{
		if (socket->BoneName == a_ParentBoneName)
		{
			a_OutSockets.Add(socket->SocketName);
		}
	}
}

UStaticMeshComponent* APlayerCharacter::AttachWeaponComponentToBone(FName a_BoneName, UStaticMesh* a_WeaponMesh)
{
	USkeletalMeshComponent* skeletonMesh = GetMesh();
	if (!skeletonMesh || !a_WeaponMesh) return nullptr;
	
	TArray<UStaticMeshComponent*> meshComponents;
	GetComponents<UStaticMeshComponent>(meshComponents);

	for (UStaticMeshComponent* component : meshComponents)
	{
		if (component->GetAttachSocketName() == a_BoneName)
		{
			component->DestroyComponent(true);
			break;
		}
	}

	UStaticMeshComponent* newComponent = NewObject<UStaticMeshComponent>(this, a_BoneName);
	newComponent->SetStaticMesh(a_WeaponMesh);

	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

	newComponent->AttachToComponent(skeletonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, a_BoneName);
	newComponent->RegisterComponent();

	m_AttachedWeapons.Add(a_BoneName, newComponent);
	return newComponent;
}

void APlayerCharacter::HideAllWeapons()
{
	for (const TPair<FName, UStaticMeshComponent*>& pair : m_AttachedWeapons)
	{
		SetWeaponVisibility(pair.Key, false);
	}
}

void APlayerCharacter::HideAllWeaponsExcept(FName a_BoneName)
{
	for (const TPair<FName, UStaticMeshComponent*>& pair : m_AttachedWeapons)
	{
		if (pair.Key != a_BoneName)
			SetWeaponVisibility(pair.Key, false);
		else SetWeaponVisibility(pair.Key, true);
	}
}

