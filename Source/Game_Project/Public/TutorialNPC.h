// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include <WMainHubTutorialNPC.h>
#include "TutorialNPC.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ATutorialNPC : public ANPC
{
	GENERATED_BODY()
	
public:
	virtual void Interact() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWMainHubTutorialNPC> m_UIClass;

	UPROPERTY()
	UWMainHubTutorialNPC* m_UI;
};
