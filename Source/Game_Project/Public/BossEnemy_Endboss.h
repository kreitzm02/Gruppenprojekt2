// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "Components/ArrowComponent.h"
#include "BossEnemy_Endboss.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ABossEnemy_Endboss : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy_Endboss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetArenaXMin(float a_xSizeMin) { m_xSizeMin = a_xSizeMin; }
	void SetArenaXMax(float a_xSizeMax) { m_xSizeMax = a_xSizeMax; }
	void SetArenaYMin(float a_ySizeMin) { m_ySizeMin = a_ySizeMin; }
	void SetArenaYMax(float a_ySizeMax) { m_ySizeMax = a_ySizeMax; }

	void SetBoundariesChecked(bool a_boundariesChecked) { m_boundariesChecked = a_boundariesChecked; }

private:
	FVector RandomVectorInBoundaries();

	FVector GetDirToPlayerNormalized();

	void ResetMoveSpeed();

	void SetCorners();

	void MoveToPlayerFurthestEdge();

	void CheckIfMovedToEdge();

	bool PlayerInRange();

	void FireProjectile(FVector a_targetDir, FVector a_spawnPoint);

	void FireProjectileWithCount(FVector a_targetDir, FVector a_spawnPoint, int a_count);

	void FireFireball(AActor* a_target, FVector a_spawnPoint);

	void FireBoulderAbovePlayer();

	void FireBurnGround(FVector a_targetPos, FVector a_spawnPoint);

	void FireRandomBurnGroundWithCount(FVector a_spawnPoint, int a_count);

	void FireRandomBurnGround();

	FTimerHandle m_periodicBoulderTimer;
	FTimerHandle m_periodicBurnGroundTimer;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_accelerationSpeed = 600.0f;


	UPROPERTY()
	AAIController* m_aiController = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_leftHandAtkAnim = nullptr;

	float m_leftHandAtkAnimLength = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_rightHandAtkAnim = nullptr;

	float m_rightHandAtkAnimLength = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_bothHandsAtkAnim = nullptr;

	float m_bothHandsAtkAnimLength = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_specialsAfterLostHPPercent = 0.2;

	float m_nextSpecialAtPercent = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxMultiplierAtHPPercent = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_doStuffMaxMultiplier = 4.0f;

	float m_currentDoStuffMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_shootAtAnimOffset = 0.5f;

	UPROPERTY(EditAnywhere)
	UArrowComponent* m_projectileSpawnLeftWeapon = nullptr;

	UPROPERTY(EditAnywhere)
	UArrowComponent* m_projectileSpawnRightWeapon = nullptr;

	FVector m_currentProjectileSpawnPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_boundariesCheckerBP = nullptr;


	//normal projectile
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_projectileBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_projectileSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_projectileLifetime = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_multiProjectileSpaceDegree = 30.0f;

	int m_multipleProjectileAtkCount = -1;


	//fireball
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_fireballBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_fireballSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_fireballLifetime = 8.0f;


	//boulder
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_boulderBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_boulderHeightOffset = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_basePeriodicBoulderSpawn = 20.0f;


	//burn ground
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_burnGroundBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_burnGroundFlightTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_burnGroundLifetime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_startFireBurnGroundAtHpPercent = 0.50f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_periodicBurnGroundTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	int m_burnGroundProjectileCount = 4;

	bool m_periodicBurnGroundStarted = false;



	float m_baseMaxMoveSpeed;

	float m_baseAcceleration;


	UPROPERTY()
	ACharacter* m_player = nullptr;

	bool m_boundariesChecked = false;

	float m_xSizeMin = 0.0f;
	float m_xSizeMax = 0.0f;
	float m_ySizeMin = 0.0f;
	float m_ySizeMax = 0.0f;

	bool m_cornersSet = false;

	FVector m_topLeft = FVector::ZeroVector;
	FVector m_topRight = FVector::ZeroVector;
	FVector m_botLeft = FVector::ZeroVector;
	FVector m_botRight = FVector::ZeroVector;

	bool m_movingToEdge = false;

	bool m_isDoingSpecialAtk = false;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_specialAtkDuration = 12.0f;

	bool m_shotFired = false;

	float m_atkTimer = 0.0f;
	float m_specialAtkTimer = 0.0f;

	bool m_doLeftHandAtk = true;

	bool m_restartAttack = true;

	bool m_needAtkInit = true;

	float m_cooldown = 1.0f;
};
