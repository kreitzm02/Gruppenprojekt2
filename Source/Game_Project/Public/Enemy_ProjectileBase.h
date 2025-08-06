// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy_ProjectileBase.generated.h"

UCLASS()
class GAME_PROJECT_API AEnemy_ProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_ProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void MoveInDirection(float a_deltaTime);
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(AActor* a_target){ m_target = a_target; }

	void SetTargetDirection(const FVector &a_target) { m_targetDirection = a_target; }

	void SetProjectileSpeed(float a_projectileSpeed){ m_projectileSpeed = a_projectileSpeed; }

	void SetProjectileLifeSpan(float a_lifeSpan) { m_lifeSpan = a_lifeSpan; }

	void SetOwnerEnemy(AEnemyCharacter* a_enemyCharacter){ m_enemyCharacter = a_enemyCharacter; }

	float GetKnockback() { return m_knockback; }

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);


protected:
	bool CheckForWallCollide();

	UPROPERTY(EditAnywhere, Category = "Debug")
	float m_enableDebug = false;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	UShapeComponent* m_projectileHitbox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	UStaticMeshComponent* m_projectileMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	FRotator m_arrowRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	FVector m_hitboxBoxSize = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float m_hitboxSphereSize = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float m_knockback = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float m_damage = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	float m_lineTraceLenght = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	FVector m_arrowSize = FVector::OneVector;

	UPROPERTY()
	AActor* m_target = nullptr;

	UPROPERTY()
	AEnemyCharacter* m_enemyCharacter = nullptr;

	FVector m_targetDirection = FVector::ForwardVector;

	float m_projectileSpeed = 1000.0f;

	float m_lifeSpan = 5.0f;
};
