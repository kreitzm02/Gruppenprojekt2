// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharDataAsset.h"
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
	void AbilitySlotIncrease();
	void AbilitySlotDecrease();

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

	// manipulate player stats
	// void TakeDamage(int32 a_Damage); redunant

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);

	void ResetStatsToDefault();
	void ChangeMovementSpeed(int32 a_Value);
	void ChangeLuck(int32 a_Value);
	void ChangeDefense(int32 a_Value);
	void ChangeAttackSpeed(int32 a_Value);

	void CheckForDeath(); // no functionality yet

	// external way to see and react to player stats
	int32 GetPlayerHealth() { return m_PlayerHealth; }
	int32 GetPlayerLuck() { return m_PlayerLuck; }
	int32 GetPlayerDefense() { return m_PlayerDefense; }
	int32 GetPlayerMovementSpeed() { return m_PlayerMovementSpeed; }
	int32 GetPlayerAttackSpeed() { return m_PlayerAttackSpeed; }

	// 
	void ChangeToPlayerClassA();
	void ChangeToPlayerClassB();
	void ChangeToPlayerClassC();
	void ChangeToPlayerClassD();
	bool CheckIfCurrentPlayerClassIsValid();

private:

	// Player Settings

	UPROPERTY(EditAnywhere, Category = "General Player Settings")
	float m_RunMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, Category = "General Player Settings")
	int32 m_AbilityNum = 4;

	int32 m_CurrentAbilitySlot = 0;

	int32 m_CurrentPlayerClass = 0;

	// player stats

	UPROPERTY(VisibleAnywhere)
	int32 m_PlayerHealth;

	UPROPERTY(VisibleAnywhere)
	int32 m_PlayerMovementSpeed;

	UPROPERTY(VisibleAnywhere)
	int32 m_PlayerAttackSpeed;

	UPROPERTY(VisibleAnywhere)
	int32 m_PlayerDefense;

	UPROPERTY(VisibleAnywhere)
	int32 m_PlayerLuck;

	//

	UPROPERTY()
	FVector m_PreviousLocation;
	EPlayerState m_PlayerState;

	// setup methods
	void SetupCamera();
	void SetupMovement();
	void SetupAbilityComp();
	void SetupPlayer();
	void SetupWeapons();

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
	UStaticMesh* m_Mace;

	UPROPERTY(EditAnywhere, Category = "Basic Weapon Meshes")
	UStaticMesh* m_Scythe;

	// weapon setup helper functions

	void GetChildBones(const FName& a_ParentBoneName, TArray<FName>& a_OutChildBones) const;

	void GetWeaponSockets(const FName& a_ParentBoneName, TArray<FName>& a_OutSockets) const;

	UStaticMeshComponent* AttachWeaponComponentToBone(FName a_BoneName, UStaticMesh* a_WeaponMesh);

	void SetupChangedPlayerClass();

	//

	UPROPERTY()
	TMap<FName, UStaticMeshComponent*> m_AttachedWeapons;

public:
	UPROPERTY(EditAnywhere, Category = "Data Asset")
	TArray<UPlayerCharDataAsset*> m_PlayerCharDataAssets;
};

UENUM()
enum class EPlayerState : uint8
{
	IDLE, WALK, SPRINT
};
