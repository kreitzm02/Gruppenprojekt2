// Fill out your copyright notice in the Description page of Project Settings.


#include "Game_GameInstance.h"

#include "LoadingScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"


void UGame_GameInstance::Init()
{
	Super::Init();

	LoadOrCreateSaveGame();
}


void UGame_GameInstance::LoadOrCreateSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0))
	{
		m_playerSave = Cast<UPlayerSave>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));
	}
	else
	{
		m_playerSave = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
		UGameplayStatics::SaveGameToSlot(m_playerSave, TEXT("PlayerSaveSlot"), 0);
	}
}

bool UGame_GameInstance::TryBuyDmgBonus()
{
	int cost = m_playerSave->CalcCostFromCurUpgrCount(m_playerSave->m_damageMultiplierCount);

	if (m_playerSave->m_currency >= cost)
	{
		m_playerSave->m_damageMultiplierCount++;
		m_playerSave->m_currency -= cost;
		UGameplayStatics::SaveGameToSlot(m_playerSave, TEXT("PlayerSaveSlot"), 0);
		return true;
	}
	return false;
}

bool UGame_GameInstance::TryBuyHPBonus()
{
	int cost = m_playerSave->CalcCostFromCurUpgrCount(m_playerSave->m_healthMultiplierCount);

	if (m_playerSave->m_currency >= cost)
	{
		m_playerSave->m_healthMultiplierCount++;
		m_playerSave->m_currency -= cost;
		UGameplayStatics::SaveGameToSlot(m_playerSave, TEXT("PlayerSaveSlot"), 0);
		return true;
	}
	return false;
}

bool UGame_GameInstance::TryBuyStaminaRegen()
{
	int cost = m_playerSave->CalcCostFromCurUpgrCount(m_playerSave->m_staminaGainMultiplierCount);

	if (m_playerSave->m_currency >= cost)
	{
		m_playerSave->m_staminaGainMultiplierCount++;
		m_playerSave->m_currency -= cost;
		UGameplayStatics::SaveGameToSlot(m_playerSave, TEXT("PlayerSaveSlot"), 0);
		return true;
	}
	return false;
}

bool UGame_GameInstance::TryBuyXPBonus()
{
	int cost = m_playerSave->CalcCostFromCurUpgrCount(m_playerSave->m_xpGainMultiplierCount);

	if (m_playerSave->m_currency >= cost)
	{
		m_playerSave->m_xpGainMultiplierCount++;
		m_playerSave->m_currency -= cost;
		UGameplayStatics::SaveGameToSlot(m_playerSave, TEXT("PlayerSaveSlot"), 0);
		return true;
	}
	return false;
}

bool UGame_GameInstance::TryBuyHPRegen()
{
	int cost = m_playerSave->CalcCostFromCurUpgrCount(m_playerSave->m_healthRegenCount);

	if (m_playerSave->m_currency >= cost)
	{
		m_playerSave->m_healthRegenCount++;
		m_playerSave->m_currency -= cost;
		UGameplayStatics::SaveGameToSlot(m_playerSave, TEXT("PlayerSaveSlot"), 0);
		return true;
	}
	return false;
}






void UGame_GameInstance::StartGameTimer()
{
	m_remainingTime = m_gameTimerDuration;

	GetWorld()->GetTimerManager().SetTimer(m_gameTimerHandle, this, &UGame_GameInstance::TickTimer, 1.0f, true);

	if (m_timerWidget)
	{
		m_timerWidgetInstance = CreateWidget<UWidget_GameTimer>(GetWorld(), m_timerWidget);
		if (m_timerWidgetInstance)
		{
			m_timerWidgetInstance->AddToViewport();
		}
	}
}

void UGame_GameInstance::AddGameTimerToViewport()
{
	m_timerWidgetInstance->AddToViewport();
}


void UGame_GameInstance::TickTimer()
{
	m_remainingTime--;
	if (m_timerWidgetInstance)
	{
		m_timerWidgetInstance->UpdateGameTime(m_remainingTime);
	}

	if (m_remainingTime <= 0)
	{
		OnTimerEnd();
	}
}

void UGame_GameInstance::OnTimerEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(m_gameTimerHandle);

	UE_LOG(LogTemp, Error, TEXT("TIMER ENDED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"))

	if (m_timerWidgetInstance)
	{
		m_timerWidgetInstance->RemoveFromParent();
		m_timerWidgetInstance->Destruct();
	}
	ACharacter* player = GetFirstLocalPlayerController()->GetCharacter();
	ULoadingScreenManager::Get(player->GetWorld())->StartLoading(player->GetWorld());
	UGameplayStatics::OpenLevel(this, "EndbossArena");
}
