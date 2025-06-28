// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Enemy_Rogue.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AEnemy_Rogue : public AEnemyCharacter
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	AEnemy_Rogue();

	virtual void Tick(float DeltaTime) override;

	float GetArrowSprayDuration() { return m_arrowSprayDuration; }

	bool GetArrowSprayReady() { return m_arrowSprayReady; }

	void SetArrowSprayReady(bool a_chargeReady) { m_arrowSprayReady = a_chargeReady; }

	void FireArrow(AActor* a_target);

	UAnimSequence* GetArrowSprayAnimation() { return m_arrowSprayAnimation; }

	UAnimSequence* GetAttackAnimation() { return m_attackAnimation; }

	UAnimSequence* GetReloadAnimation() { return m_reloadAnimation; }

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_arrowSprayAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_attackAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UAnimSequence* m_reloadAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowSprayDuration = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowSprayCooldown = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float m_arrowLifetime = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	UClass* m_arrowBP = nullptr;

	UPROPERTY(EditAnywhere)
	UArrowComponent* m_projectileSpawnPoint = nullptr;

	float m_passedCooldownTime = 0.0f;

	bool m_arrowSprayReady = true;
};
