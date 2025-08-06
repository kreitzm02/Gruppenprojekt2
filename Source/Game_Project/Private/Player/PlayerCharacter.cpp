// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "AbilityComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "LoadingScreenManager.h"
#include "MainAbilityContainer.h"
#include "GameFramework/Controller.h"
#include "NPC.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include <Kismet/GameplayStatics.h>

#include "EnemyCharacter.h"
#include <Game_GameInstance.h>

#pragma region UNREAL METHODS
// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupCamera();
	SetupAbilityComp();
	SetupMeleeHitbox();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ULoadingScreenManager::Get(GetWorld())->EndLoading();

	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}

	EnableInput(Cast<APlayerController>(GetController()));

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	m_PlayerIsInMainhub = LevelName == "MainHub1";

	// TODO: Create LoadFromSaveFile()
	UGame_GameInstance* gameInstance = Cast<UGame_GameInstance>(GetGameInstance());
	m_CurrentPlayerClass = gameInstance->m_playerSave->m_CurrentPlayerClass;
	
	FillAbilityLevelMap();

	if (LevelName == "EndbossArena" || LevelName == "MainHub1" || LevelName == "TutorialArea1") // keep abilities when entering these levels
	{
		for (int i = 0; i < gameInstance->m_playerSave->m_AbilityClasses.Num(); i++)
		{
			AddAbilityDirect(gameInstance->m_playerSave->m_AbilityClasses[i]);
		}
		m_AbilityLevels = gameInstance->m_playerSave->m_AbilityLevels;
	}
	
	SetupPlayer();

	m_AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	   
	m_PreviousLocation = GetActorLocation();

	SetupWeapons();
	HideAllWeapons();
	ChangeToAbilitySlot0(); // the char equips his default ability
	SetupMovement();
	m_AbilityCooldownTimes.Init(0.0f, m_AbilityMax);
	m_AbilityMaxCooldownTimes.Init(0.0f, m_AbilityMax);
	m_AbilityIcons.Init(nullptr, m_AbilityMax);
	m_AbilityNames.Init(FText::FromString(TEXT("Empty")), m_AbilityMax);
	m_LvlUpAbilitySelection.Init(nullptr, 3);

	if (m_playerUI)
	{
		m_playerUIInstance = CreateWidget<UWidget_PlayerUI>(GetWorld(),m_playerUI);
		m_playerUIInstance->AddToViewport();
		m_playerUIInstance->UpdateShownMoney();
		gameInstance->SetPlayerUI(m_playerUIInstance);
	}

	m_PlayerTotalExp = m_PlayerLevelExp = 0;
	m_ExpForLevelUp = m_ExpLevelBarrier = 250;
	m_PlayerLvl = 1;


	if (gameInstance->TimerWidgetNotNullptr())
	{
		gameInstance->AddGameTimerToViewport();
	}
}

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
		m_AnimInstance->m_IsRunning = m_PlayerShouldSprint && m_PlayerStamina > 2.0f;
		m_AnimInstance->m_IsWalking = (m_PlayerShouldSprint && m_PlayerStamina <= 1.0f) || (!m_PlayerShouldSprint && speed > 0.1f);
		// to avoid t posing when no montage is playing in upper body slot
		float targetWeight = m_AnimInstance->Montage_IsPlaying(nullptr) ? 1.0f : 0.0f;
		m_AnimInstance->m_BlendWeight = FMath::FInterpTo(m_AnimInstance->m_BlendWeight, targetWeight, DeltaTime, 9.5f);
		m_AnimInstance->m_PlayerAlive = m_IsPlayerAlive;
	}

	CheckForDeath();

	for (int i = 0; i < m_AbilityMax; i++)
	{
		m_AbilityCooldownTimes[i] = m_PlayerAbilities->GetRemainingCooldownFromAbility(i);
		m_AbilityIcons[i] = m_PlayerAbilities->GetAbilityIcon(i);
		m_AbilityNames[i] = m_PlayerAbilities->GetAbilityName(i);
		UE_LOG(LogTemp, Warning, TEXT("Slot: %i, Ability: %s"), i, *m_PlayerAbilities->GetAbilityName(i).ToString())
		m_AbilityMaxCooldownTimes[i] = m_PlayerAbilities->GetAbilityCooldown(i);
	}
	UpdatePlayerStamina(DeltaTime, speed, movementThreshold);
	UpdatePlayerSpeed();

	switch (m_AbilityMax)
	{
	case 1:
		m_playerUIInstance->SetAbilityOneCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(0) / m_PlayerAbilities->GetAbilityCooldown(0));
		break;
	case 2:
		m_playerUIInstance->SetAbilityOneCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(0) / m_PlayerAbilities->GetAbilityCooldown(0));
		m_playerUIInstance->SetAbilityTwoCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(1) / m_PlayerAbilities->GetAbilityCooldown(1));
		break;
	case 3:
		m_playerUIInstance->SetAbilityOneCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(0) / m_PlayerAbilities->GetAbilityCooldown(0));
		m_playerUIInstance->SetAbilityTwoCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(1) / m_PlayerAbilities->GetAbilityCooldown(1));
		m_playerUIInstance->SetAbilityThreeCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(2) / m_PlayerAbilities->GetAbilityCooldown(2));
		break;
	case 4:
		m_playerUIInstance->SetAbilityOneCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(0) / m_PlayerAbilities->GetAbilityCooldown(0));
		m_playerUIInstance->SetAbilityTwoCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(1) / m_PlayerAbilities->GetAbilityCooldown(1));
		m_playerUIInstance->SetAbilityThreeCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(2) / m_PlayerAbilities->GetAbilityCooldown(2));
		m_playerUIInstance->SetAbilityFourCooldownPercent(m_PlayerAbilities->GetRemainingCooldownFromAbility(3) / m_PlayerAbilities->GetAbilityCooldown(3));
		break;
	default:
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player Abilities: %i"), m_AbilityNum)

	m_PlayerMaxHealth = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseHealthPoints * Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->GetPlayerHPMultiplier();

	if (!m_playerIsHittable)
	{
		m_passedInvulnarabilityTime += DeltaTime;
		if (m_passedInvulnarabilityTime >= m_invulnarabilityTime)
		{
			m_playerIsHittable = true;
		}
	}
}
#pragma endregion

#pragma region INPUT

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
	m_PlayerShouldSprint = true;
}

void APlayerCharacter::StopSprint()
{
	m_PlayerShouldSprint = false;
}

void APlayerCharacter::UseAbility()
{
	if (m_PlayerIsInMainhub)
	{
		InteractWithNearbyNPC();
	}
	else
	{
		m_PlayerAbilities->ActivateAbility(m_CurrentAbilitySlot);
	}
}

void APlayerCharacter::ChangeToAbilitySlot0()
{
	ChangeToAbilitySlot(0);
}

void APlayerCharacter::ChangeToAbilitySlot1()
{
	if (m_AbilityNum < 2) return;
	ChangeToAbilitySlot(1);
}

void APlayerCharacter::ChangeToAbilitySlot2()
{
	if (m_AbilityNum < 3) return;
	ChangeToAbilitySlot(2);
}

void APlayerCharacter::ChangeToAbilitySlot3()
{
	if (m_AbilityNum < 4) return;
	ChangeToAbilitySlot(3);
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
	//testing only!!
	AddExperiencePoints(200);
	//m_CurrentPlayerClass = 3;
	//SetupChangedPlayerClass();
}

void APlayerCharacter::ActivateMouseCursor(bool a_SetActive)
{
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc && a_SetActive)
	{
		pc->bShowMouseCursor = true;
		pc->SetInputMode(FInputModeUIOnly());
	}
	else if (pc && !a_SetActive)
	{
		pc->bShowMouseCursor = false;
		pc->SetInputMode(FInputModeGameOnly());
	}
}

void APlayerCharacter::ActivatePauseMenu()
{
	if (m_PauseUI && !m_PauseUIInstance)
	{
		m_PauseUIInstance = CreateWidget<UWidget_PauseMenu>(GetWorld(), m_PauseUI);
	}

	ActivateMouseCursor(true);
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(m_PauseUIInstance->TakeWidget());
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetInputMode(InputMode);
	m_PauseUIInstance->AddToViewport(2);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

#pragma endregion

#pragma region SETUP

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
	PlayerInputComponent->BindAction("PlayerClassD", IE_Pressed, this, &APlayerCharacter::ChangeToPlayerClassD);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::ActivatePauseMenu);
}

void APlayerCharacter::SetupChangedPlayerClass()
{
	GetMesh()->SetSkeletalMesh(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_Mesh);
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	if (m_MeleeHitBox)
	{
		m_MeleeHitBox->SetupAttachment(GetMesh(), TEXT("hand_r"));
		m_MeleeHitBox->SetBoxExtent({ 1.5f,0.4f,0.4f });
		m_MeleeHitBox->SetRelativeLocation({ -0.8f, 0.0f, 0.0f });
		m_MeleeHitBox->SetVisibility(false);
		m_MeleeHitBox->SetHiddenInGame(true);
	}

	m_AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	m_PlayerAbilities->RemoveAllAbilities();
	AddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility);
	m_CurrentAbilitySlot = 0;
	Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->m_CurrentPlayerClass = m_CurrentPlayerClass; 

	// setup the player stats with the default values from the given data asset
	ResetStatsToDefault();
	ChangeToAbilitySlot0();
}

void APlayerCharacter::SetupCamera()
{
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraBoom->SetupAttachment(RootComponent);

	m_PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	m_PlayerCamera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
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
	AddAbility(m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_StartingAbility);

	if (m_MeleeHitBox)
	{
		m_MeleeHitBox->SetupAttachment(GetMesh(), TEXT("hand_r"));
		m_MeleeHitBox->SetBoxExtent({ 1.5f,0.4f,0.4f });
		m_MeleeHitBox->SetRelativeLocation({ -0.8f, 0.0f, 0.0f });
		m_MeleeHitBox->SetVisibility(false);
		m_MeleeHitBox->SetHiddenInGame(true);
	}

	m_AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	// setup the player stats with the default values from the given data asset
	ResetStatsToDefault();
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
		else if (boneName == TEXT("2H_Mace")) meshAsset = m_DruidStaff;
		else if (boneName == TEXT("2H_Staff")) meshAsset = m_Staff;
		else if (boneName == TEXT("1H_Dagger")) meshAsset = m_Dagger;
		else if (boneName == TEXT("1H_Scythe")) meshAsset = m_Spellbook;
		else continue;

		UStaticMeshComponent* meshComponent = AttachWeaponComponentToBone(boneName, meshAsset);
		if (!meshComponent) continue;
		meshComponent->SetCanEverAffectNavigation(false);
	}
}

void APlayerCharacter::SetupMeleeHitbox()
{
	m_MeleeHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeHitBox"));
	m_MeleeHitBox->SetupAttachment(GetMesh(), TEXT("hand_r"));
	m_MeleeHitBox->SetBoxExtent({ 1.5f,0.4f,0.4f });
	m_MeleeHitBox->SetRelativeLocation({ -0.8f, 0.0f, 0.0f });
	m_MeleeHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_MeleeHitBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	m_MeleeHitBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	m_MeleeHitBox->SetVisibility(false);
	m_MeleeHitBox->SetHiddenInGame(true);
	m_MeleeHitBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnHit);
}

#pragma endregion

#pragma region LEVEL

void APlayerCharacter::UpdatePlayerLevel()
{
	m_PlayerLvl++;
	m_PlayerLevelExp = FMath::Max(0, m_PlayerLevelExp - m_ExpLevelBarrier);
	if (m_PlayerLvl < 25)
	{
		m_ExpLevelBarrier *= 1 + (((100 - 4 * m_PlayerLvl) / m_PlayerLvl) / 100);
		m_ExpForLevelUp *= 1 + (((100 - 4 * m_PlayerLvl) / m_PlayerLvl) / 100);
	}
	m_playerUIInstance->SetExpPercent(float(m_PlayerLevelExp) / m_ExpForLevelUp);
	for (int i = 0; i < m_LvlUpAbilitySelection.Num(); i++)
	{
		m_LvlUpAbilitySelection[i] = GetRandomAbilityFromPool();
	}
	ToggleLvlUpUI(true);
	UE_LOG(LogTemp, Warning, TEXT("Player reached Level: %i"), m_PlayerLvl);
	UE_LOG(LogTemp, Warning, TEXT("Player experience points: %i"), m_PlayerLevelExp);
}

#pragma endregion

#pragma region WEAPON

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
	m_CurrentAttackDamage = 0.0f;
	UE_LOG(LogTemp, Error, TEXT("AAAAAAAAAAAAAAA"))
}

void APlayerCharacter::ShowMeleeHitbox()
{
	m_CurrentAttackDamage = m_AttackDamage * Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->m_damageMultiplier + Cast<UGame_GameInstance>(GetGameInstance())->m_AdditionalDamage;
	UE_LOG(LogTemp, Error, TEXT("BBBBBBBBBBBBBB"))
}

#pragma endregion

#pragma region FIGHT

void APlayerCharacter::ClearAlreadyHitActors()
{
	m_AlreadyHitActors.Empty();
}

// GETTING DAMAGE

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float totalDmg = DamageAmount * (100 - m_PlayerDefense) / 100;
	if (m_playerIsHittable)
	{
		UE_LOG(LogTemp, Warning, TEXT("player got damage: %f"), totalDmg)
		UE_LOG(LogTemp, Warning, TEXT("Player health: %f"), m_PlayerHealth)
		TryAddPlayerHealth(totalDmg * -1); // "adds" negative health
		m_passedInvulnarabilityTime = 0.0f;
		m_playerIsHittable = false;
	}
	if (DamageCauser)
	{
		FVector knockbackDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		knockbackDirection.Z = 0;
		knockbackDirection.Normalize();

		AEnemyCharacter* enemy = Cast<AEnemyCharacter>(EventInstigator->GetCharacter());
		HandleKnockback(knockbackDirection, enemy->GetKnockback());
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
		UGameplayStatics::ApplyDamage(a_otherActor, m_CurrentAttackDamage, GetController(), this, nullptr);
		if (AEnemyCharacter* hitEnemy = Cast<AEnemyCharacter>(a_otherActor))
		{
			hitEnemy->TakeKnockback(m_KnockbackStrenght, hitEnemy->GetActorLocation() - GetActorLocation());
		}
	}
}

void APlayerCharacter::CheckForDeath()
{
	if (m_PlayerHealth <= 0)
	{
		m_IsPlayerAlive = false;
		DisableInput(Cast<APlayerController>(GetController()));
		FTimerHandle deathDelayTimer;
		float deathDelayBeforeRespawn = 3.0f;
		GetWorld()->GetTimerManager().SetTimer(deathDelayTimer, FTimerDelegate::CreateUObject(this, &APlayerCharacter::RespawnAfterDeath), deathDelayBeforeRespawn, false);
	}
}

void APlayerCharacter::RespawnAfterDeath()
{
	ULoadingScreenManager::Get(GetWorld())->StartLoading(GetWorld());
	UGameplayStatics::OpenLevel(this, FName("MainHub1"));
}

#pragma endregion

#pragma region UI

void APlayerCharacter::UpdateHealthBar()
{
	float healthPercentage = m_PlayerHealth / m_PlayerMaxHealth;
	m_playerUIInstance->SetHealthPercent(healthPercentage);
}

void APlayerCharacter::UpdateStaminabar()
{
	float staminaPercentage = m_PlayerStamina / m_PlayerMaxStamina;
	m_playerUIInstance->SetStaminaPercent(staminaPercentage);
}

void APlayerCharacter::ToggleLvlUpUI(bool a_SetActive)
{
	if (m_lvlUpUI && !m_lvlUpUIInstance)
	{
		m_lvlUpUIInstance = CreateWidget<UWLvlUpAbilitySelect>(GetWorld(), m_lvlUpUI);
	}

	if (!a_SetActive)
	{
		ActivateMouseCursor(false);
		m_lvlUpUIInstance->RemoveFromViewport();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
	else
	{
		ActivateMouseCursor(true);
		m_lvlUpUIInstance->AddToViewport(2);
		m_lvlUpUIInstance->SetButtonTexts(
			FText::FromString(m_LvlUpAbilitySelection[0]->m_DisplayName),
			FText::FromString(m_LvlUpAbilitySelection[1]->m_DisplayName),
			FText::FromString(m_LvlUpAbilitySelection[2]->m_DisplayName));
		m_lvlUpUIInstance->SetButtonImages(
			m_LvlUpAbilitySelection[0]->m_Icon,
			m_LvlUpAbilitySelection[1]->m_Icon,
			m_LvlUpAbilitySelection[2]->m_Icon);
		m_lvlUpUIInstance->SetLevelUpText(FText::FromString(FString::Printf(TEXT("You have reached Level %i!"), m_PlayerLvl)));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
	}
}

void APlayerCharacter::ToggleLvlUpReplaceUI(bool a_SetActive)
{
	if (m_lvlUpReplaceUI && !m_lvlUpReplaceUIInstance)
	{
		m_lvlUpReplaceUIInstance = CreateWidget<UWLvlUpAbilityReplace>(GetWorld(), m_lvlUpReplaceUI);
	}

	if (!a_SetActive)
	{
		ActivateMouseCursor(false);
		m_lvlUpReplaceUIInstance->RemoveFromViewport();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
	else
	{
		ActivateMouseCursor(true);
		m_lvlUpReplaceUIInstance->AddToViewport(2);
		m_lvlUpReplaceUIInstance->SetButtonTexts(
			m_PlayerAbilities->GetAbilityName(0), m_PlayerAbilities->GetAbilityName(1),
			m_PlayerAbilities->GetAbilityName(2), m_PlayerAbilities->GetAbilityName(3));
		m_lvlUpReplaceUIInstance->SetButtonImages(
			m_PlayerAbilities->GetAbilityIcon(0), m_PlayerAbilities->GetAbilityIcon(2),
			m_PlayerAbilities->GetAbilityIcon(1), m_PlayerAbilities->GetAbilityIcon(3));
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
	}
}

#pragma endregion

#pragma region ABILITIES

void APlayerCharacter::SetAbilityOneIcon()
{
	m_playerUIInstance->SetAbilityOneIcon(m_PlayerAbilities->GetAbilityIcon(0));
	GetWorld()->GetTimerManager().ClearTimer(m_setAbilityOneIconTimer);
}

void APlayerCharacter::SetAbilityTwoIcon()
{
	m_playerUIInstance->SetAbilityTwoIcon(m_PlayerAbilities->GetAbilityIcon(1));
	GetWorld()->GetTimerManager().ClearTimer(m_setAbilityTwoIconTimer);
}

void APlayerCharacter::SetAbilityThreeIcon()
{
	m_playerUIInstance->SetAbilityThreeIcon(m_PlayerAbilities->GetAbilityIcon(2));
	GetWorld()->GetTimerManager().ClearTimer(m_setAbilityThreeIconTimer);
}

void APlayerCharacter::SetAbilityFourIcon()
{
	m_playerUIInstance->SetAbilityFourIcon(m_PlayerAbilities->GetAbilityIcon(3));
	GetWorld()->GetTimerManager().ClearTimer(m_setAbilityFourIconTimer);
}



void APlayerCharacter::FillAbilityLevelMap()
{
	if (!m_AbilityLevels.IsEmpty()) return; // if we have a savegame, we can sync the map with the save file before calling this method.
	for (int i = 0; i < (int)EAllAbilities::ENUMLENGTH; i++)
	{
		int level = 1;
		m_AbilityLevels.Add(EAllAbilities(i), level);
	}
}

UMainAbilityContainerDataAsset* APlayerCharacter::GetRandomAbilityFromPool()
{
	TArray<UMainAbilityContainerDataAsset*> tempPool = m_AbilityPool;
	UMainAbilityContainerDataAsset* randAbility = nullptr;
	for (int i = 0; i < m_AbilityPool.Num(); i++)
	{
		int32 randIndex = FMath::RandRange(0, tempPool.Num() - 1);
		if (m_AbilityLevels.FindRef(tempPool[randIndex]->m_ThisAbility) == 3 && PlayerHasAbility(tempPool[randIndex]))
		{
			tempPool.RemoveAt(randIndex);
			continue;
		}
		randAbility = tempPool[randIndex];
		break;
	}
	return randAbility;
}

bool APlayerCharacter::PlayerHasAbility(UMainAbilityContainerDataAsset* a_Ability)
{
	EAllAbilities abilityType = a_Ability->GetAbilityType();
	int abilityLvl = m_AbilityLevels.FindRef(abilityType);
	TSubclassOf<UBaseAbility> newAbility = a_Ability->GetAbility(abilityLvl);
	for (UBaseAbility* ability : m_PlayerAbilities->m_Abilities)
	{
		if (ability && ability->GetClass() == newAbility) return true;
	}
	return false;
}

void APlayerCharacter::AddAbilityDirect(TSubclassOf<UBaseAbility> a_Ability)
{
	if (m_AbilityNum >= m_AbilityMax) return;
	m_PlayerAbilities->TryAddAbility(a_Ability);
	m_AbilityNum++;
}

void APlayerCharacter::AddAbility(UMainAbilityContainerDataAsset* a_Ability, bool a_IncreaseAbilityCount)
{
	EAllAbilities abilityType = a_Ability->GetAbilityType();
	int abilityLvl = m_AbilityLevels.FindRef(abilityType);
	TSubclassOf<UBaseAbility> ability = a_Ability->GetAbility(abilityLvl);

	if (PlayerHasAbility(a_Ability))
	{
		ChangeAbilityLevel(abilityType, 1);

		int index = m_AbilityLevels.FindRef(abilityType);

		if (index == abilityLvl) return; // ability is already lvl3
		m_PlayerAbilities->RemoveAbility(ability); // removing the lower lvl ability
		AddAbility(a_Ability, false); // adding the new ability
		Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->m_Abilities = m_PlayerAbilities->m_Abilities;
		Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->m_AbilityClasses = m_PlayerAbilities->m_AbilityClasses;
		return;
	}
	m_PlayerAbilities->TryAddAbility(ability);
	if (a_IncreaseAbilityCount) m_AbilityNum++;
	
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityOneIconTimer, this, &APlayerCharacter::SetAbilityOneIcon, 0.2f, false);
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityTwoIconTimer, this, &APlayerCharacter::SetAbilityTwoIcon, 0.2f, false);
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityThreeIconTimer, this, &APlayerCharacter::SetAbilityThreeIcon, 0.2f, false);
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityFourIconTimer, this, &APlayerCharacter::SetAbilityFourIcon, 0.2f, false);


	Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->m_Abilities = m_PlayerAbilities->m_Abilities;
	Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->m_AbilityClasses = m_PlayerAbilities->m_AbilityClasses;
	return;
}

void APlayerCharacter::ReplaceAbility(UMainAbilityContainerDataAsset* a_NewAbility, int a_OldAbilitySlot)
{
	if (m_AbilityNum < m_AbilityMax) // player has less than 4 abilities
	{
		AddAbility(a_NewAbility, true);
		return;
	}
	EAllAbilities abilityType = a_NewAbility->GetAbilityType();
	int abilityLvl = m_AbilityLevels.FindRef(abilityType);
	TSubclassOf<UBaseAbility> ability = a_NewAbility->GetAbility(abilityLvl);
	m_PlayerAbilities->RemoveAbilityFromIndex(a_OldAbilitySlot);
	m_AbilityNum--;
	AddAbility(a_NewAbility, true);
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityOneIconTimer, this, &APlayerCharacter::SetAbilityOneIcon, 0.2f, false);
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityTwoIconTimer, this, &APlayerCharacter::SetAbilityTwoIcon, 0.2f, false);
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityThreeIconTimer, this, &APlayerCharacter::SetAbilityThreeIcon, 0.2f, false);
	GetWorld()->GetTimerManager().SetTimer(m_setAbilityFourIconTimer, this, &APlayerCharacter::SetAbilityFourIcon, 0.2f, false);
}

void APlayerCharacter::ChangeAbilityLevel(EAllAbilities a_Ability, int a_Value)
{
	if (!m_AbilityLevels.Find(a_Ability) || m_AbilityLevels[a_Ability] >= 3) return;
	
	m_AbilityLevels[a_Ability] += a_Value;

	UGame_GameInstance* gameInstance = Cast<UGame_GameInstance>(GetGameInstance());
	gameInstance->m_playerSave->m_AbilityLevels = m_AbilityLevels;
}

void APlayerCharacter::ChangeToAbilitySlot(int32 a_Index)
{
	m_CurrentAbilitySlot = a_Index;
	m_AttackDamage = m_PlayerCharDataAssets[m_CurrentPlayerClass]->m_BaseAttackPoints; // reset current attack damage
	m_PlayerAbilities->EquipAbility(a_Index); // new damage will be set here if needed
}

void APlayerCharacter::AbilitySlotIncrease()
{
	if (m_CurrentAbilitySlot == m_AbilityMax - 1) m_CurrentAbilitySlot = 0;
	else if (m_CurrentAbilitySlot >= m_AbilityMax || m_CurrentAbilitySlot <= -1) m_CurrentAbilitySlot = 0;
	else m_CurrentAbilitySlot++;
	m_PlayerAbilities->EquipAbility(m_CurrentAbilitySlot);
}

void APlayerCharacter::AbilitySlotDecrease()
{
	if (m_CurrentAbilitySlot == 0) m_CurrentAbilitySlot = m_AbilityMax - 1;
	else if (m_CurrentAbilitySlot >= m_AbilityMax || m_CurrentAbilitySlot <= -1) m_CurrentAbilitySlot = 0;
	else m_CurrentAbilitySlot--;
	m_PlayerAbilities->EquipAbility(m_CurrentAbilitySlot);
}

void APlayerCharacter::AddAbilityFromUI(int a_Index)
{
	UE_LOG(LogTemp, Warning, TEXT("AddAbilityFromUI - Called"));
	m_AbilityToAddIndex = a_Index;
	if (m_AbilityNum < m_AbilityMax || PlayerHasAbility(m_LvlUpAbilitySelection[m_AbilityToAddIndex]))
	{
		UE_LOG(LogTemp, Warning, TEXT("AddAbilityFromUI - Calling AddAbility"));
		AddAbility(m_LvlUpAbilitySelection[m_AbilityToAddIndex], true);
	}
	else
	{
		ToggleLvlUpUI(false);
		ToggleLvlUpReplaceUI(true);
		return;
	}
	ToggleLvlUpUI(false);
}

void APlayerCharacter::ReplaceAbilityFromUI(int a_IndexToReplace)
{
	ReplaceAbility(m_LvlUpAbilitySelection[m_AbilityToAddIndex], a_IndexToReplace);
	ToggleLvlUpReplaceUI(false);
}

#pragma endregion

#pragma region STATS

void APlayerCharacter::AddExperiencePoints(int a_Amount)
{
	m_PlayerTotalExp += a_Amount * Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->GetPlayerXPGainMultiplier();
	m_PlayerLevelExp += a_Amount * Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->GetPlayerXPGainMultiplier();

	m_playerUIInstance->SetExpPercent(float(m_PlayerLevelExp) / m_ExpForLevelUp);

	if (m_PlayerLevelExp - m_ExpForLevelUp >= 0)
	{
		UpdatePlayerLevel();
	}
}



void APlayerCharacter::TryAddPlayerHealth(float a_Amount)
{
	if (m_PlayerHealth + a_Amount >= m_PlayerMaxHealth) m_PlayerHealth = m_PlayerMaxHealth;
	else m_PlayerHealth += a_Amount;
	UpdateHealthBar();
}



bool APlayerCharacter::CheckIfCurrentPlayerClassIsValid()
{
	if (m_PlayerCharDataAssets.Num() > m_CurrentPlayerClass && m_CurrentPlayerClass >= 0) return true;
	else return false;
}

// PLAYER STATS

void APlayerCharacter::ResetStatsToDefault()
{
	UPlayerCharDataAsset* currentPlayerDA = m_PlayerCharDataAssets[m_CurrentPlayerClass];
	m_PlayerHealth = currentPlayerDA->m_BaseHealthPoints;
	m_PlayerMaxHealth = currentPlayerDA->m_BaseHealthPoints;
	m_PlayerMovementSpeed = currentPlayerDA->m_BaseMoveSpeed;
	m_PlayerDefense = currentPlayerDA->m_BaseDefense;
	m_PlayerLuck = currentPlayerDA->m_BaseLuck;
	m_PlayerAttackSpeed = currentPlayerDA->m_BaseAttackSpeed;
	m_PlayerStamina = currentPlayerDA->m_BaseStamina;
	m_PlayerMaxStamina = currentPlayerDA->m_BaseStamina;
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

void APlayerCharacter::ChangePlayerStamina(float a_Value)
{
	m_PlayerStamina += a_Value;
	if (m_PlayerStamina > m_PlayerMaxStamina)
	{
		m_PlayerStamina = m_PlayerMaxStamina;
	}
	UpdateStaminabar();
}

void APlayerCharacter::UpdatePlayerSpeed()
{
	if (m_PlayerStamina > 0 && m_PlayerShouldSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = m_PlayerMovementSpeed * m_RunMultiplier;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = m_PlayerMovementSpeed;
	}
}

void APlayerCharacter::UpdatePlayerStamina(float a_DeltaTime, float a_Speed, float a_MovementThreshold)
{
	if (a_Speed > m_PlayerMovementSpeed + a_MovementThreshold)
	{
		ChangePlayerStamina(-12.5f * a_DeltaTime);
	}
	else
	{
		ChangePlayerStamina(8.0f * Cast<UGame_GameInstance>(GetGameInstance())->m_playerSave->GetPlayerStaminaRegenMultiplier() * a_DeltaTime);
	}
}

void APlayerCharacter::SetCurrentPlayerClass(int a_ClassIndex)
{
	m_CurrentPlayerClass = a_ClassIndex;
	SetupChangedPlayerClass();
	UGame_GameInstance* gameInstance = Cast<UGame_GameInstance>(GetGameInstance());
	gameInstance->m_playerSave->m_CurrentPlayerClass = m_CurrentPlayerClass;
}

#pragma endregion

void APlayerCharacter::InteractWithNearbyNPC()
{
	TArray<AActor*> NPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), NPCs);

	for (AActor* Actor : NPCs)
	{
		ANPC* NPC = Cast<ANPC>(Actor);
		if (NPC && NPC->IsPlayerInRange() && NPC->m_OverlappingPlayer == this)
		{
			NPC->Interact();
			return;
		}
	}
}
