// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Warrior.h"

void AEnemy_Warrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!chargeReady && passedCooldownTime <= chargeCooldown)
	{
		passedCooldownTime += DeltaTime;

		if (passedCooldownTime >= chargeCooldown)
		{
			chargeReady = true;
			passedCooldownTime = 0.0f;
		}
	}
}
