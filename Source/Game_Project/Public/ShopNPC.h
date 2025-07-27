// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "ShopNPC.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API AShopNPC : public ANPC
{
	GENERATED_BODY()
public:
	virtual void Interact() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> m_UIClass;

	UPROPERTY()
	UUserWidget* m_UI;
};
