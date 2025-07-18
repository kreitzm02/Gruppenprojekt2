// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGM.h"
#include "Player/PlayerCharacter.h"
#include "DefaultPlayerController.h"

ATestGM::ATestGM()
{
	DefaultPawnClass = m_PlayerPawnBPClass;
	PlayerControllerClass = ADefaultPlayerController::StaticClass();
}
