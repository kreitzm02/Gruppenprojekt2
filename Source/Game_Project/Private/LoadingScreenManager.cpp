// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include <MoviePlayer.h>

ULoadingScreenManager* ULoadingScreenManager::Get(UWorld* a_World)
{
    static ULoadingScreenManager* instance = nullptr;
    
    if (!instance)
    {
        instance = NewObject<ULoadingScreenManager>();
        instance->SetFlags(RF_Transient);
        instance->AddToRoot();
    }
    instance->m_World = a_World;
    instance->m_LoadingScreenWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/LoadingScreen/WBP_Loading.WBP_Loading_C")));
    
    return instance;
}

void ULoadingScreenManager::StartLoading(UWorld* a_World)
{
    FLoadingScreenAttributes attributes;
    attributes.bAutoCompleteWhenLoadingCompletes = true;
    attributes.MinimumLoadingScreenDisplayTime = 2.0f;
    UClass * WidgetClass = m_LoadingScreenWidgetClass.LoadSynchronous();
    m_LoadingWidget = CreateWidget<UUserWidget>(m_World, WidgetClass);
    attributes.WidgetLoadingScreen = m_LoadingWidget->TakeWidget();
    attributes.PlaybackType = EMoviePlaybackType::MT_LoadingLoop;

    GetMoviePlayer()->SetupLoadingScreen(attributes);
    GetMoviePlayer()->PlayMovie();
}

void ULoadingScreenManager::EndLoading()
{
    GetMoviePlayer()->StopMovie();
    //if (m_LoadingWidget)
    //{
    //    m_World->GetTimerManager().SetTimer(m_EndLoadingDelayTimer, FTimerDelegate::CreateUObject(this, &ULoadingScreenManager::EndLoadingDelegate), 0.1f, false);
    //}
}

void ULoadingScreenManager::EndLoadingDelegate()
{
    m_LoadingWidget->RemoveFromParent();
    m_LoadingWidget = nullptr;
    m_World->GetTimerManager().ClearTimer(m_EndLoadingDelayTimer);
}
