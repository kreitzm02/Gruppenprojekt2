// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "Enemy_GolemBoulder.generated.h"

UCLASS()
class GAME_PROJECT_API AEnemy_GolemBoulder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_GolemBoulder();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOwner(AEnemyCharacter* a_owner) { m_owner = a_owner; }

	UFUNCTION()
	void OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);

	UFUNCTION()
	void OnHitPhysically(UPrimitiveComponent* a_hitComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, FVector a_normalImpulse, const FHitResult& a_hit);
private:

	void DestroyBoulder();

	void DestroyBoulderWithShockwave();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_meshComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* m_underBoulderHitbox = nullptr;

	UPROPERTY()
	AEnemyCharacter* m_owner;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UNiagaraSystem* m_boulderDestroyNiagara = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UClass* m_shockwaveBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shockwaveExpansionSpeed = 120.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shockwaveMaxRadius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float m_shockwaveStartRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector m_hitboxScale = FVector::OneVector;
};
