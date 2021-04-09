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
}

void APiouPiouBangGameMode::BeginPlay() {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), AllSpawnPoint);
}

void APiouPiouBangGameMode::CactusSpawn()
{
	FVector position = AllSpawnPoint[FMath::RandRange(0, AllSpawnPoint.Num() - 1)]->GetActorLocation();
	ACactus* cac = GetWorld()->SpawnActor<ACactus>(Cactus, position, FRotator::ZeroRotator);
	AllCactus.Add(cac);
}

void APiouPiouBangGameMode::Tick(float dt)
{
	SpawnCooldown -= dt;
	if (SpawnCooldown <= 0) {
		CactusSpawn();
		SpawnCooldown += BaseSpawnCooldown;
	}

	for (int i = AllCactus.Num() - 1; i >= 0; i--) {
		ACactus* cac = AllCactus[i];
		if (cac->GetBoolDespawn()) {
			AllCactus.RemoveAt(i);
			cac->Destroy();
		}
	}
}

TArray<ACactus*>& APiouPiouBangGameMode::GetAllCactus()
{
	return AllCactus;
}
