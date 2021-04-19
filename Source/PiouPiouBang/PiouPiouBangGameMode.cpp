// Copyright Epic Games, Inc. All Rights Reserved.

#include "PiouPiouBangGameMode.h"
#include "PiouPiouBangHUD.h"
#include "PiouPiouBangCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Cactus.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "SpawnPoint.h"
#include "Math/UnrealMathUtility.h"

APiouPiouBangGameMode::APiouPiouBangGameMode()
	: Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APiouPiouBangHUD::StaticClass();

	static ConstructorHelpers::FObjectFinder<UClass> CactusClassFinder(TEXT("Blueprint'/Game/Ennemy/Cactus/Blueprint/BP_Cactus.BP_Cactus_C'"));
	Cactus = CactusClassFinder.Object;

	VagueNumber = 0;
	WaveEnded = true;
}

void APiouPiouBangGameMode::BeginPlay() {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), AllSpawnPoint);
	StartWave();
}

void APiouPiouBangGameMode::CactusSpawn()
{
	FVector position = AllSpawnPoint[FMath::RandRange(0, AllSpawnPoint.Num() - 1)]->GetActorLocation();
	ACactus* cac = GetWorld()->SpawnActor<ACactus>(Cactus, position, FRotator::ZeroRotator);
	AllCactus.Add(cac);
}

void APiouPiouBangGameMode::Tick(float dt)
{
	if (WaveEnded) {
		StartWave();
	}
	else {
		SpawnCooldown -= dt;
		if (CactusToSpawn>0 && SpawnCooldown <= 0) {
			CactusSpawn();
			SpawnCooldown += BaseSpawnCooldown;
			CactusToSpawn--;
		}

		for (int i = AllCactus.Num() - 1; i >= 0; i--) {
			ACactus* cac = AllCactus[i];
			if (cac->GetBoolDespawn()) {
				AllCactus.RemoveAt(i);
				cac->Destroy();
			}
		}

		if (AllCactus.Num() == 0 && CactusToSpawn==0) WaveEnded = true;
	}
}

void APiouPiouBangGameMode::StartWave() {
	WaveEnded = false;
	VagueNumber++;
	CactusToSpawn = StartingEnnemieNumber + (VagueNumber-1) * AdditionalEnnemieNumber;
	SpawnCooldown += BaseSpawnCooldown;
}

void APiouPiouBangGameMode::AddScore(int score)
{
	Score += score;
	APiouPiouBangHUD* hud = Cast<APiouPiouBangHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->SetScore(Score);
}



TArray<ACactus*>& APiouPiouBangGameMode::GetAllCactus()
{
	return AllCactus;
}
