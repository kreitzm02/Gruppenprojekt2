// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "TutorialAreaLevelUpNPC.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ATutorialAreaLevelUpNPC : public ANPC
{
	GENERATED_BODY()

public:
	virtual void Interact() override;
};
