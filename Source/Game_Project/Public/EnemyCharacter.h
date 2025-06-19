// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void HandleKnockback(FVector a_knockbackDirection, float a_knockbackStrength);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);

	UAnimSequence* GetIdleAnimation() { return idleAnimation; }

	UAnimSequence* GetWalkAnimation() { return walkAnimation; }

	UAnimSequence* GetChaseAnimation() { return chaseAnimation; }

	UAnimSequence* GetDeathAnimation() { return deathAnimation; }

	float GetPlayerDetectionRadius() { return playerDetectionRadius; }

	float GetPlayerChaseRadius() { return playerChaseRadius; }

	float GetIdleWalkSpeed() { return idleWalkSpeed; }

	float GetChaseWalkSpeed() { return chaseWalkSpeed; }

	float GetAttackRange() { return attackRange; }

	float GetCurrentHealth() { return currentHealth; }

	void SetDeathState(bool a_isDead) { isDead = a_isDead; }
private:
	bool isDead = false;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(VisibleAnywhere)
	UFSM_EnemyStateMachineComponent* stateMachine;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Properties")
	class UBoxComponent* hitbox;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* idleAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* walkAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* chaseAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* deathAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float playerDetectionRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float playerChaseRadius = 1100.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float idleWalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float chaseWalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float attackRange = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float attackDamage = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float maxHealth = 100.0f;

	float currentHealth;
};
