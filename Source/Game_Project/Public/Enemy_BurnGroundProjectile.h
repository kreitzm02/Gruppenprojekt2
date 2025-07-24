// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy_BurnGroundProjectile.generated.h"

UCLASS()
class GAME_PROJECT_API AEnemy_BurnGroundProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_BurnGroundProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetOwner(AEnemyCharacter* a_owner) { m_owner = a_owner; }

	void SetTargetPos(FVector a_targetPos) { m_projectileTargetPos = a_targetPos; }

	void SetFlightTime(float a_flightTime) { m_flightTime = a_flightTime; }

	void SetLifetime(float a_lifetime) { m_lifetime = a_lifetime; }

private:
	FTimerHandle m_triggerTimerHandle;

	UPROPERTY()
	AActor* m_overlappedActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool m_enableDebug = true;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_flyingCurveHeight = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_hitboxSize = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector m_burnGroundScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UNiagaraComponent* m_niagaraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UNiagaraSystem* m_projectileFlightNiagara = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UNiagaraSystem* m_burnGroundNiagara = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_possibleDamageIntervalInSec = 2.0f;

	float m_passedTimeLastDmgInterval = 10.0f;

	UPROPERTY()
	AEnemyCharacter* m_owner = nullptr;

	FVector m_projectileStartPos = FVector::OneVector;

	FVector m_projectileTargetPos = FVector::OneVector;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_hitbox = nullptr;

	float m_lifetime = 1.0f;

	float m_flightTime = 1.0f;

	float m_ellapsedTime = 0.0f;

	bool m_burnGroundCreated = false;
};
