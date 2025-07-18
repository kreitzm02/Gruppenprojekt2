// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoadingScreenManager.generated.h"

/**
 * 
 */
UCLASS()
class GAME_PROJECT_API ULoadingScreenManager : public UObject
{
	GENERATED_BODY()
	
public:
	static ULoadingScreenManager* Get(UWorld* a_World);
	void StartLoading(UWorld* a_World);
	void EndLoading();

private:
	UPROPERTY()
	UUserWidget* m_LoadingWidget;

	TSoftClassPtr<UUserWidget> m_LoadingWidgetClass;

	FTimerHandle m_EndLoadingDelayTimer;

	UWorld* m_World;

	void EndLoadingDelegate();
};
