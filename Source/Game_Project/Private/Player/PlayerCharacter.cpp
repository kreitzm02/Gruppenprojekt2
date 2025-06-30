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
#include <Kismet/GameplayStatics.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupCamera();
	SetupAbilityComp();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupPlayer();

	m_AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	m_PreviousLocation = GetActorLocation();

	SetupWeapons();
	SetupMeleeHitbox();
	HideAllWeapons();
	ChangeToAbilitySlot0(); // the char equips his default ability
	SetupMovement();
	m_AbilityCooldownTimes.Init(0.0f, m_AbilityNum);
}

// INPUT 

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
	GetCharacterMovement()->MaxWalkSpeed = m_PlayerMovementSpeed * m_RunMultiplier;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = m_PlayerMovementSpeed;
}

void APlayerCharacter::UseAbility()
{
	m_PlayerAbilities->ActivateAbility(m_CurrentAbilitySlot); 
}

void APlayerCharacter::ChangeToAbilitySlot0()
{
	ChangeToAbilitySlot(0);
}

void APlayerCharacter::ChangeToAbilitySlot1()
{
	ChangeToAbilitySlot(1);
}

void APlayerCharacter::ChangeToAbilitySlot2()
{
	ChangeToAbilitySlot(2);
}

void APlayerCharacter::ChangeToAbilitySlot3()
{
	ChangeToAbilitySlot(3);
}

void APlayerCharacter::ChangeToAbilitySlot(int32 a_Index)
{
	m_CurrentAbilitySlot = a_Index;
	m_AttackDamage = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseAttackPoints; // reset current attack damage
	m_PlayerAbilities->EquipAbility(a_Index); // new damage will be set here if needed
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

//

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CheckIfCurrentPlayerClassIsValid())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Fatal error: Current player class index is not valid!"));
	}

	const FVector currentLocation = GetActorLocation();
	const float speed = FVector::Dist(currentLocation, m_PreviousLocation) / DeltaTime;
	m_PreviousLocation = currentLocation;
	const float movementThreshold = 0.0f;

	if (m_AnimInstance)
	{
		m_AnimInstance->m_IsRunning = speed > m_PlayerMovementSpeed + movementThreshold ;
		m_AnimInstance->m_IsWalking = speed > movementThreshold && speed <= m_PlayerMovementSpeed + movementThreshold;
		// to avoid t posing when no montage is playing in upper body slot
		float targetWeight = m_AnimInstance->Montage_IsPlaying(nullptr) ? 1.0f : 0.0f;
		m_AnimInstance->m_BlendWeight = FMath::FInterpTo(m_AnimInstance->m_BlendWeight, targetWeight, DeltaTime, 9.5f);
		m_AnimInstance->m_PlayerAlive = m_IsPlayerAlive;
	}
	
	CheckForDeath();

	if (UAnimMontage* activeMontage = m_AnimInstance->GetCurrentActiveMontage())
	{
		float progress = m_AnimInstance->Montage_GetPosition(activeMontage) / activeMontage->GetPlayLength();

		if (progress > 0.9f)
		{
			HideMeleeHitbox();
		}
	}

	for (int i = 0; i < m_AbilityNum; i++)
	{
		m_AbilityCooldownTimes[i] = m_PlayerAbilities->GetRemainingCooldownFromAbility(i);
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
	PlayerInputComponent->BindAction("PlayerClassA", IE_Pressed, this, &APlayerCharacter::ChangeToPlayerClassA);
	PlayerInputComponent->BindAction("PlayerClassB", IE_Pressed, this, &APlayerCharacter::ChangeToPlayerClassB);
	PlayerInputComponent->BindAction("PlayerClassC", IE_Pressed, this, &APlayerCharacter::ChangeToPlayerClassC);
	PlayerInputComponent->BindAction("PlayerClassD", IE_Pressed, this, &APlayerCharacter::ChangeToPlayerClassD);
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
	GetCharacterMovement()->MaxWalkSpeed = m_PlayerMovementSpeed;

}

void APlayerCharacter::SetupAbilityComp()
{
	m_PlayerAbilities = CreateDefaultSubobject<UAbilityComponent>(TEXT("PlayerAbilities"));
}

void APlayerCharacter::SetupPlayer()
{
	GetMesh()->SetSkeletalMesh(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_Mesh);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility);

	// only for debug purposes. the player would normally start with just 1 ability ( the m_StartingAbility)
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility1Debug);
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility2Debug);
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility3Debug);

	// setup the player stats with the default values from the given data asset
	ResetStatsToDefault();
} 

// WEAPONS

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

void APlayerCharacter::SetupMeleeHitbox()
{
	m_MeleeHitBox = NewObject<UBoxComponent>(this, TEXT("MeleeHitBox"));
	m_MeleeHitBox->SetupAttachment(GetMesh(), TEXT("handslot_r"));
	m_MeleeHitBox->SetBoxExtent(FVector(0.25f, 0.25f, 0.25f));
	m_MeleeHitBox->SetRelativeLocation(FVector(0.0f, -0.57f, -0.1f));
	m_MeleeHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeleeHitBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	m_MeleeHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_MeleeHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	m_MeleeHitBox->RegisterComponent();

	m_MeleeHitBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnHit);
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

	newComponent->AttachToComponent(skeletonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("handslot_r"));
	newComponent->RegisterComponent();
	newComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 90.0f));

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

void APlayerCharacter::HideMeleeHitbox()
{
	m_MeleeHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::ShowMeleeHitbox()
{
	m_MeleeHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::ClearAlreadyHitActors()
{
	m_AlreadyHitActors.Empty();
}

// GETTING DAMAGE

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float totalDmg = DamageAmount * (100 - m_PlayerDefense) / 100;
	m_PlayerHealth -= totalDmg;
	if (m_PlayerHealth <= 0) m_PlayerHealth = 0;
	//UE_LOG(LogTemp, Warning, TEXT("Player was hit"))
		if (DamageCauser)
		{
			FVector knockbackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
			knockbackDirection.Z = 0;
			knockbackDirection.Normalize();
			HandleKnockback(knockbackDirection, 0.0f /*get knockback strengh from damage causer*/);
			//ACharacter* damagingUnit = Cast<ACharacter>(DamageCauser);
			//damagingUnit->GetKnockback();
		}

	return totalDmg;
}

void APlayerCharacter::HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength)
{
	LaunchCharacter(a_knockbackDirection * a_knockbackStrength, true, true);
}

void APlayerCharacter::OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult)
{
	if (a_otherActor && a_otherActor != this && a_otherComp)
	{
		if (m_AlreadyHitActors.Contains(a_otherActor)) return;
		m_AlreadyHitActors.Add(a_otherActor);
		UGameplayStatics::ApplyDamage(a_otherActor, m_AttackDamage, GetController(), this, nullptr);
	}
}

void APlayerCharacter::CheckForDeath()
{
	if (m_PlayerHealth <= 0)
	{
		m_IsPlayerAlive = false;
		DisableInput(Cast<APlayerController>(GetController()));
	}
}

// PLAYER CLASS

void APlayerCharacter::SetupChangedPlayerClass()
{
	GetMesh()->SetSkeletalMesh(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_Mesh);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	m_PlayerAbilities->RemoveAllAbilities();
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility);

	// only for debug purposes. the player would normally start with just 1 ability ( the m_StartingAbility)
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility1Debug);
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility2Debug);
	m_PlayerAbilities->TryAddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility3Debug);

	// setup the player stats with the default values from the given data asset
	ResetStatsToDefault();
	ChangeToAbilitySlot0();
}

void APlayerCharacter::ChangeToPlayerClassA()
{
	m_CurrentPlayerClass = 0;
	SetupChangedPlayerClass();
}

void APlayerCharacter::ChangeToPlayerClassB()
{
	m_CurrentPlayerClass = 1;
	SetupChangedPlayerClass();
}

void APlayerCharacter::ChangeToPlayerClassC()
{
	m_CurrentPlayerClass = 2;
	SetupChangedPlayerClass();
}

void APlayerCharacter::ChangeToPlayerClassD()
{
	m_CurrentPlayerClass = 3;
	SetupChangedPlayerClass();
}

bool APlayerCharacter::CheckIfCurrentPlayerClassIsValid()
{
	if (m_PlayerCharDataAssets.Num() > m_CurrentPlayerClass && m_CurrentPlayerClass >= 0) return true;
	else return false;
}

// PLAYER STATS

void APlayerCharacter::ResetStatsToDefault()
{
	m_PlayerHealth = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseHealthPoints;
	m_PlayerMovementSpeed = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseMoveSpeed;
	m_PlayerDefense = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseDefense;
	m_PlayerLuck = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseLuck;
	m_PlayerAttackSpeed = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseAttackSpeed;
}

void APlayerCharacter::ChangeMovementSpeed(float a_Value)
{
	m_PlayerMovementSpeed += a_Value;
	if (m_PlayerMovementSpeed <= 0) m_PlayerMovementSpeed = 0;
}

void APlayerCharacter::ChangeLuck(float a_Value)
{
	m_PlayerLuck += a_Value;
	if (m_PlayerLuck <= 0) m_PlayerLuck = 0;
}

void APlayerCharacter::ChangeDefense(float a_Value)
{
	m_PlayerDefense += a_Value;
	m_PlayerDefense = FMath::Clamp(m_PlayerDefense, 0, 100); // defense must be between 0 and 100 as it  decreases incoming damageby that percentage.
}

void APlayerCharacter::ChangeAttackSpeed(float a_Value)
{
	m_PlayerAttackSpeed += a_Value;
	if (m_PlayerAttackSpeed <= 0) m_PlayerAttackSpeed = 0;
}

void APlayerCharacter::ChangeAttackDamage(float a_Value)
{
	m_AttackDamage += a_Value;
}

