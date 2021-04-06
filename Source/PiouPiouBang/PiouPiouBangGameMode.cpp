// Copyright Epic Games, Inc. All Rights Reserved.

#include "PiouPiouBangGameMode.h"
#include "PiouPiouBangHUD.h"
#include "PiouPiouBangCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Cactus.h"

APiouPiouBangGameMode::APiouPiouBangGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APiouPiouBangHUD::StaticClass();

	
}

//GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &APiouPiouBangGameMode::SpawnUnCactus, 4, true);

void APiouPiouBangGameMode::AddCactus(ACactus* cactus)
{
	AllCactus.Add(cactus);
}

void APiouPiouBangGameMode::CactusSpawn(FVector position)
{
	GetWorld()->SpawnActor<ACactus>( position, FRotator::ZeroRotator);
}

void APiouPiouBangGameMode::SpawnUnCactus()
{
	FVector vect(0, 0, 50);
	CactusSpawn(vect);
}

void APiouPiouBangGameMode::Tick(float dt)
{
	for (int i = AllCactus.GetSlack() - 1; i >= 0; i--) {
		ACactus* cac = AllCactus[i];
		if (cac->GetBoolDespawn()) delete cac;
	}
}
