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

public:
	APiouPiouBangGameMode();

	void AddCactus(ACactus* cactus);
	void CactusSpawn(FVector position);
	void SpawnUnCactus();

	void Tick(float dt) override;

private:
	FTimerHandle SpawnTimer;
	TArray<ACactus*> AllCactus;
};



