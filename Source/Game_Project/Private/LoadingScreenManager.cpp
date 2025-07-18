// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ULoadingScreenManager* ULoadingScreenManager::Get(UWorld* World)
{
    static ULoadingScreenManager* instance = nullptr;

    if (!instance)
    {
        instance = NewObject<ULoadingScreenManager>();
        instance->m_World = World;
        instance->AddToRoot();
        instance->m_LoadingWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/LoadingScreen/WBP_Loading.WBP_Loading_C")));
        UE_LOG(LogTemp, Warning, TEXT("Loading screen not found!!!!"));
    }

    return instance;
}

void ULoadingScreenManager::StartLoading(UWorld* a_World)
{
    if (!m_LoadingWidget)
    {
        UClass* WidgetClass = m_LoadingWidgetClass.LoadSynchronous();
        if (WidgetClass)
        {
            m_LoadingWidget = CreateWidget<UUserWidget>(m_World, WidgetClass);
            if (m_LoadingWidget)
            {
                m_LoadingWidget->AddToViewport(100);
                UE_LOG(LogTemp, Warning, TEXT("Loading screen widget added to viewport"));
            }
        }
    }
}

void ULoadingScreenManager::EndLoading()
{
    if (m_LoadingWidget)
    {
        m_World->GetTimerManager().SetTimer(m_EndLoadingDelayTimer, FTimerDelegate::CreateUObject(this, &ULoadingScreenManager::EndLoadingDelegate), 2.0f, false);
    }
}

void ULoadingScreenManager::EndLoadingDelegate()
{
    m_LoadingWidget->RemoveFromParent();
    m_LoadingWidget = nullptr;
    m_World->GetTimerManager().ClearTimer(m_EndLoadingDelayTimer);
}
