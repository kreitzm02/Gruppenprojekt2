// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonExitPortal.h"
#include "EnemyCharacter.h"
#include "BossEnemy_Rogue.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ABossEnemy_Rogue : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	ABossEnemy_Rogue();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaTime) override;

	float GetArrowSprayDuration() { return m_arrowSprayDuration; }

	float GetArrowCircleDuration() { return m_arrowCircleDuration; }

	bool GetArrowSprayReady() { return m_arrowSprayReady; }

	void SetArrowSprayReady(bool a_chargeReady) { m_arrowSprayReady = a_chargeReady; }

	bool GetArrowCircleReady() { return m_arrowCircleReady; }

	void SetArrowCircleReady(bool a_chargeReady) { m_arrowCircleReady = a_chargeReady; }

	float GetCurrentMultiplier() { return m_currentDoStuffMultiplier; }

	void FireArrow(AActor* a_target);

	void FireArrow(const FVector& a_targetDir);

	//FVector GetSpawnPoint() { return m_spawnPoint; }

	UAnimSequence* GetAttackWOReloadAnimation() { return m_attackWithoutReloadAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

	UAnimSequence* GetReloadAnimation() { return m_reloadAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Scale")
	FVector m_weaponScale = FVector::OneVector;

	//UPROPERTY()
	//FVector m_spawnPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackWithoutReloadAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_reloadAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowSprayDuration = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowCircleDuration = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_abilityCooldown = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowLifetime = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_arrowBP = nullptr;

	UPROPERTY(EditAnywhere)
	UArrowComponent* m_projectileSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_maxMultiplierAtHPPercent = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_doStuffMaxMultiplier = 4.0f;

	float m_currentDoStuffMultiplier = 1.0f;

	float m_passedCooldownTime = 0.0f;

	bool m_arrowSprayReady = false;

	bool m_arrowCircleReady = false;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	TSubclassOf<ADungeonExitPortal> m_dungeonExitBP = nullptr;
};
