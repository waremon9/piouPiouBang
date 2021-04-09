// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PiouPiouBangGameMode.generated.h"

class ACactus;

UCLASS(minimalapi)
class APiouPiouBangGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TSubclassOf<ACactus> Cactus;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float SpawnCooldown;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float BaseSpawnCooldown;

public:
	APiouPiouBangGameMode();

	void CactusSpawn();

	void BeginPlay() override;
	void Tick(float dt) override;

	void AddScore(int score);
	int GetScore() const;

	TArray<ACactus*>& GetAllCactus();

private:
	TArray<ACactus*> AllCactus;
	TArray<AActor*> AllSpawnPoint;

	float Score;
};



