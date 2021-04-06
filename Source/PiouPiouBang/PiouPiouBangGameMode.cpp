// Copyright Epic Games, Inc. All Rights Reserved.

#include "PiouPiouBangGameMode.h"
#include "PiouPiouBangHUD.h"
#include "PiouPiouBangCharacter.h"
#include "UObject/ConstructorHelpers.h"

APiouPiouBangGameMode::APiouPiouBangGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APiouPiouBangHUD::StaticClass();
}
