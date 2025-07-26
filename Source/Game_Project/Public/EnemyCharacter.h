// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_EnemyHealthBar.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UFSM_EnemyStateMachineComponent;

UCLASS()
class GAME_PROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);

	UAnimSequence* GetIdleAnimation() { return m_idleAnimation; }

	UAnimSequence* GetWalkAnimation() { return m_walkAnimation; }

	UAnimSequence* GetChaseAnimation() { return m_chaseAnimation; }

	UAnimSequence* GetDeathAnimation() { return m_deathAnimation; }

	UBoxComponent* GetWeaponHitbox() { return m_weaponHitbox; }

	float GetPlayerDetectionRadius() { return m_playerDetectionRadius; }

	float GetPlayerChaseRadius() { return m_playerChaseRadius; }

	float GetIdleWalkSpeed() { return m_idleWalkSpeed; }

	float GetChaseWalkSpeed() { return m_chaseWalkSpeed; }

	float GetAttackRange() { return m_attackRange; }

	float GetCurrentHealth() { return m_currentHealth; }

	float GetAttackDuration() { return m_attackDuration; }

	float GetAttackDamage() { return m_attackDamage; }

	float GetKnockback() { return m_knockback; }

	void SetDeathState(bool a_isDead) { m_isDead = a_isDead; }
protected:
	void UpdateHealthBar();

	virtual void OnDeath();

	bool m_isDead = false;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* m_skeletalMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_weaponMesh;

	UPROPERTY(VisibleAnywhere)
	UFSM_EnemyStateMachineComponent* m_stateMachine;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Properties")
	UBoxComponent* m_characterHitbox;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Properties")
	UBoxComponent* m_weaponHitbox;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_idleAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_walkAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_chaseAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_deathAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_playerDetectionRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_playerChaseRadius = 1100.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_idleWalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_chaseWalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_attackRange = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_attackDamage = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_knockback = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int m_moneyValueOnDeath = 5;

	UPROPERTY(EditAnywhere, Category = "Weapon Rotation")
	FRotator m_weaponRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetComponent* m_healthBarComponent = nullptr;

	UPROPERTY()
	UWidget_EnemyHealthBar* m_widgetHealthBar = nullptr;

	float m_currentHealth;

	float m_attackDuration = 0.0f;
};
