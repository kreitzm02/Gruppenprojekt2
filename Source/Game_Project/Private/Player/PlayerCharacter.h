// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharDataAsset.h"
#include <Components/BoxComponent.h>
#include "PlayerAnimInstance.h"
#include "AllAbilities.h"
#include "Widget_PlayerUI.h"
#include "Blueprint/UserWidget.h"
#include <WLvlUpAbilitySelect.h>
#include "WLvlUpAbilityReplace.h"
#include <Widget_PauseMenu.h>
#include "PlayerCharacter.generated.h"

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* m_CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* m_PlayerCamera;

	UPROPERTY(VisibleAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class UAbilityComponent* m_PlayerAbilities;

	// called from input
	void MoveForward(float a_Value);
	void MoveRight(float a_Value);
	void StartSprint();
	void StopSprint();
	void UseAbility();
	void ChangeToAbilitySlot0();
	void ChangeToAbilitySlot1();
	void ChangeToAbilitySlot2();
	void ChangeToAbilitySlot3();
	void ChangeToAbilitySlot(int32 a_Index);
	void AbilitySlotIncrease();
	void AbilitySlotDecrease();
	void ActivatePauseMenu();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void SetWeaponVisibility(FName a_BoneName, bool a_SetVisible);
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void HideAllWeapons();
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void HideAllWeaponsExcept(FName a_BoneName);

	void HideMeleeHitbox();
	void ShowMeleeHitbox();
	void ClearAlreadyHitActors();

	// take and deal damage

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);
	
	// manipulate player stats

	void ResetStatsToDefault();
	void ChangeMovementSpeed(float a_Value);
	void ChangeLuck(float a_Value);
	void ChangeDefense(float a_Value);
	void ChangeAttackSpeed(float a_Value);
	void ChangeAttackDamage(float a_Value);
	void ChangePlayerStamina(float a_Value);

	void CheckForDeath();
	void RespawnAfterDeath();

	// external way to see and react to player stats
	float GetPlayerHealth() { return m_PlayerHealth; }
	float GetPlayerLuck() { return m_PlayerLuck; }
	float GetPlayerDefense() { return m_PlayerDefense; }
	float GetPlayerMovementSpeed() { return m_PlayerMovementSpeed; }
	float GetPlayerAttackSpeed() { return m_PlayerAttackSpeed; }
	float GetPlayerAttackDamage() { return m_AttackDamage; }
	float GetPlayerStamina() { return m_PlayerStamina; }
	void TryAddPlayerHealth(float a_Amount);
	

	// 
	void ChangeToPlayerClassA();
	void ChangeToPlayerClassB();
	void ChangeToPlayerClassC();
	void ChangeToPlayerClassD();
	bool CheckIfCurrentPlayerClassIsValid();

private:

	void UpdateHealthBar();
	void UpdateStaminabar();
	void UpdatePlayerSpeed();
	void UpdatePlayerStamina(float a_DeltaTime, float a_Speed, float a_MovementThreshold);

	TSet<AActor*> m_AlreadyHitActors;

	TArray<float> m_AbilityCooldownTimes;

	TArray<UTexture2D*> m_AbilityIcons;

	TArray<FText> m_AbilityNames;

	TArray<float> m_AbilityMaxCooldownTimes;

	UPROPERTY()
	UBoxComponent* m_MeleeHitBox;

	// Player Settings

	UPROPERTY(EditAnywhere, Category = "General Player Settings")
	float m_RunMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, Category = "General Player Settings")
	int32 m_AbilityMax = 4;

	int32 m_AbilityNum = 0;

	int32 m_CurrentAbilitySlot = 0;

	int32 m_CurrentPlayerClass = 0;

	bool m_IsPlayerAlive = true;

	bool m_PlayerShouldSprint = false;

	bool m_PlayerIsInMainhub = false;

	void SetCurrentPlayerClass(int a_ClassIndex);

	// player stats

	UPROPERTY(VisibleAnywhere)
	float m_PlayerMaxHealth;

	UPROPERTY(VisibleAnywhere)
	float m_PlayerMaxStamina;

	UPROPERTY(VisibleAnywhere)
	float m_PlayerHealth;

	UPROPERTY(VisibleAnywhere)
	float m_PlayerMovementSpeed;

	UPROPERTY(VisibleAnywhere)
	float m_PlayerAttackSpeed;

	UPROPERTY(VisibleAnywhere)
	float m_PlayerDefense;

	UPROPERTY(VisibleAnywhere)
	float m_PlayerLuck;

	UPROPERTY(VisibleAnywhere)
	float m_AttackDamage;

	UPROPERTY(VisibleAnywhere)
	float m_PlayerStamina;

	// leveling

	UPROPERTY(VisibleAnywhere)
	int m_PlayerTotalExp;

	UPROPERTY(VisibleAnywhere)
	int m_PlayerLevelExp;

	UPROPERTY(VisibleAnywhere)
	int m_ExpForLevelUp;

	UPROPERTY(VisibleAnywhere)
	int m_ExpLevelBarrier;

	UPROPERTY(VisibleAnywhere)
	int m_PlayerLvl;

	UFUNCTION()
	void UpdatePlayerLevel();

	bool m_PlayerShouldLevelUp;

	//

	UPROPERTY()
	FVector m_PreviousLocation;

	// setup methods
	void SetupCamera();
	void SetupMovement();
	void SetupAbilityComp();
	void SetupPlayer();
	void SetupWeapons();
	void SetupMeleeHitbox();

	// weapon meshes
	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Axe;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Crossbow;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Dagger;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Spellbook;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Staff;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Wand;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Sword;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Blade;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_DruidStaff;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Scythe;

	UPROPERTY()
	TMap<FName, UStaticMeshComponent*> m_AttachedWeapons;

	// weapon setup helper functions

	void GetChildBones(const FName& a_ParentBoneName, TArray<FName>& a_OutChildBones) const;

	void GetWeaponSockets(const FName& a_ParentBoneName, TArray<FName>& a_OutSockets) const;

	UStaticMeshComponent* AttachWeaponComponentToBone(FName a_BoneName, UStaticMesh* a_WeaponMesh);

	void SetupChangedPlayerClass();

	// ui

	FTimerHandle m_setAbilityOneIconTimer;

	FTimerHandle m_setAbilityTwoIconTimer;

	FTimerHandle m_setAbilityThreeIconTimer;

	FTimerHandle m_setAbilityFourIconTimer;

	void SetAbilityOneIcon();

	void SetAbilityTwoIcon();

	void SetAbilityThreeIcon();

	void SetAbilityFourIcon();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> m_playerUI = nullptr;

	UPROPERTY()
	UWidget_PlayerUI* m_playerUIInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UWidget_PauseMenu> m_PauseUI = nullptr;

	UPROPERTY()
	UWidget_PauseMenu* m_PauseUIInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UWLvlUpAbilitySelect> m_lvlUpUI = nullptr;

	UPROPERTY()
	UWLvlUpAbilitySelect* m_lvlUpUIInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UWLvlUpAbilityReplace> m_lvlUpReplaceUI = nullptr;

	UPROPERTY()
	UWLvlUpAbilityReplace* m_lvlUpReplaceUIInstance = nullptr;

	void ActivateMouseCursor(bool a_SetActive);

	// abilities

	TMap<EAllAbilities, int> m_AbilityLevels;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<UMainAbilityContainerDataAsset*> m_AbilityPool;

	UFUNCTION()
	void FillAbilityLevelMap();

	UFUNCTION()
	void ToggleLvlUpUI(bool a_SetActive);

	UFUNCTION()
	void ToggleLvlUpReplaceUI(bool a_SetActive);

	UFUNCTION()
	UMainAbilityContainerDataAsset* GetRandomAbilityFromPool();

	UFUNCTION()
	bool PlayerHasAbility(UMainAbilityContainerDataAsset* a_Ability);

	UPROPERTY()
	TArray<UMainAbilityContainerDataAsset*> m_LvlUpAbilitySelection;

	int32 m_AbilityToAddIndex;
	
public:

	UFUNCTION()
	void AddAbilityDirect(TSubclassOf<UBaseAbility> a_Ability);

	UFUNCTION()
	void AddAbility(UMainAbilityContainerDataAsset* a_Ability, bool a_IncreaseAbilityCount = true);

	UFUNCTION()
	void ReplaceAbility(UMainAbilityContainerDataAsset* a_NewAbility, int a_OldAbilitySlot);

	TArray<float> GetPlayerAbilityCooldownTimes() { return m_AbilityCooldownTimes; }

	TArray<FText> GetPlayerAbilityNames() { return m_AbilityNames; }

	TArray<UTexture2D*> GetPlayerAbilityIcons() { return m_AbilityIcons; }

	TArray<float> GetPlayerAbilityMaxCooldownTimes() { return m_AbilityMaxCooldownTimes; }

	void ChangeAbilityLevel(EAllAbilities a_Ability, int a_Value);

	//

	void AddExperiencePoints(int a_Amount);

	void InteractWithNearbyNPC();

	// ui callable

	void AddAbilityFromUI(int a_Index);

	void ReplaceAbilityFromUI(int a_IndexToReplace);

	// player presets

	UPROPERTY(EditAnywhere, Category = "Data Asset")
	TArray<UPlayerCharDataAsset*> m_PlayerCharDataAssets;

	// anim montages

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* m_SliceAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* m_StabAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* m_ChopAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* m_ShootSpellcastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* m_RaiseSpellcastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* m_LongSpellcastMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float m_KnockbackStrenght = 700.0f;

	UPROPERTY()
	UPlayerAnimInstance* m_AnimInstance;
};
