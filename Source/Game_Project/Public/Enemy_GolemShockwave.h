// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Enemy_GolemShockwave.generated.h"
UCLASS()
class GAME_PROJECT_API AEnemy_GolemShockwave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_GolemShockwave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleVolumeChanged(float a_newVolume);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOwnerEnemy(AEnemyCharacter* a_enemyCharacter){ m_enemyCharacter = a_enemyCharacter; }

	void SetExpansionSpeed(float a_expansioSpeed){ m_expansionSpeed = a_expansioSpeed; }

	void SetMaxRadius(float a_maxRadius){ m_maxRadius = a_maxRadius; }

	void SetStartRadius(float a_startRadius){ m_currentRadius = a_startRadius; }

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool m_enableDebug = true;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UNiagaraComponent* m_shockwaveVFX = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings")
	USphereComponent* m_hitbox = nullptr;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* m_soundComp;
	
	float m_maxRadius = 200.0f;
	
	float m_expansionSpeed = 50.0f;

	float m_currentRadius = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	USoundWave* m_shockwaveStartSound = nullptr;

	UPROPERTY()
	AEnemyCharacter* m_enemyCharacter = nullptr;

	bool m_hitPlayer = false;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* a_overlappedComponent, AActor* a_otherActor, UPrimitiveComponent* a_otherComp, int32 a_otherBodyIndex, bool a_bFromSweep, const FHitResult& a_sweepResult);

};
