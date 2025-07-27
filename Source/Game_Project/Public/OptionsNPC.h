// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include <Widget_OptionsNPC.h>
#include "OptionsNPC.generated.h"


/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AOptionsNPC : public ANPC
{
	GENERATED_BODY()
public:
	virtual void Interact() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWidget_OptionsNPC> m_UIClass;

	UPROPERTY()
	UWidget_OptionsNPC* m_UI;
};
