// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include <WMainHubClassNPC.h>
#include "ClassNPC.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AClassNPC : public ANPC
{
	GENERATED_BODY()
public:
	virtual void Interact() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWMainHubClassNPC> m_UIClass;

	UPROPERTY()
	UWMainHubClassNPC* m_UI;
};
