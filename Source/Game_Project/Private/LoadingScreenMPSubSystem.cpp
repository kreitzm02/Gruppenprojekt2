// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenMPSubSystem.h"
#include "Kismet/GameplayStatics.h"                 
#include "Widgets/SWeakWidget.h"          
#include "Widgets/Layout/SBorder.h"      
#include "Widgets/Images/SThrobber.h" 
#include "MoviePlayer.h"

ULoadingScreenMPSubSystem::ULoadingScreenMPSubSystem()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("/Game/LoadingScreen/WBP_Loading.WBP_Loading_C"));
	if (Finder.Succeeded())
	{
		m_LoadingScreenWidgetClass = Finder.Class;
	}
}

void ULoadingScreenMPSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULoadingScreenMPSubSystem::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULoadingScreenMPSubSystem::HandlePostLoadMap);
}

void ULoadingScreenMPSubSystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Deinitialize();
}

void ULoadingScreenMPSubSystem::StartLoadingScreen()
{
	FLoadingScreenAttributes attributes;
	attributes.bAutoCompleteWhenLoadingCompletes = true;
	attributes.MinimumLoadingScreenDisplayTime = 2.0f;
	m_OverlayWidget = CreateWidget<UUserWidget>(GetGameInstance(), m_LoadingScreenWidgetClass);
	attributes.WidgetLoadingScreen = m_OverlayWidget->TakeWidget();
	attributes.PlaybackType = EMoviePlaybackType::MT_LoadingLoop;

	GetMoviePlayer()->SetupLoadingScreen(attributes);
	GetMoviePlayer()->PlayMovie();
}

void ULoadingScreenMPSubSystem::HandlePreLoadMap(const FString& a_MapName)
{
	StartLoadingScreen();
}

void ULoadingScreenMPSubSystem::HandlePostLoadMap(UWorld* s_LoadedWorld)
{
	
}