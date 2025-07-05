// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "TrainingsDummyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ATrainingsDummyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	ATrainingsDummyCharacter();

protected:
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnDeath() override;

	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_animation = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_outOfCombatResetTime = 0.0f;

	float m_passedCombatTime = 0.0f;

	bool m_inCombat = false;

	FVector m_defaultPosition = FVector::ZeroVector;
};
