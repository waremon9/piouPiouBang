// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PiouPiouBangHUD.generated.h"

UCLASS()
class APiouPiouBangHUD : public AHUD
{
	GENERATED_BODY()


public:
	APiouPiouBangHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;


private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	class UFont* Font;

	float Score;
};

