// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenMPSubSystem.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class GAME_PROJECT_API ULoadingScreenMPSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	ULoadingScreenMPSubSystem();

	UPROPERTY(EditDefaultsOnly) 
	TSubclassOf<UUserWidget> m_LoadingScreenWidgetClass;

	UFUNCTION(BlueprintCallable)
	void StartLoadingScreen();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void HandlePreLoadMap(const FString& a_MapName);
	void HandlePostLoadMap(UWorld* a_LoadedWorld);

private:
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> m_OverlayWidget = nullptr;
};
