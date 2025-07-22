// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAbilityContainerDataAsset.h"

TSubclassOf<UBaseAbility> UMainAbilityContainerDataAsset::GetAbility(int a_Lvl)
{
	switch (a_Lvl)
	{
		case 1: return m_LVL1;
		case 2: return m_LVL2;
		case 3: return m_LVL3;
		default: return nullptr;
	}
}
