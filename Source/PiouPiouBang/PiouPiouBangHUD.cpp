// Copyright Epic Games, Inc. All Rights Reserved.

#include "PiouPiouBangHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"

APiouPiouBangHUD::APiouPiouBangHUD()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	ConstructorHelpers::FObjectFinder<UFont> FontObject(TEXT("Font'/Game/Font/MarioLuigi2_Font.MarioLuigi2_Font'"));
	Font = FontObject.Object;
}


void APiouPiouBangHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X - 10),(Center.Y - 10));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);


	FString Text = "COUCOU";
	FLinearColor Color = FLinearColor::Red;
	float x = 50;
	float y = 50;
	float Scale = 48;
	DrawText
	(
		Text,
		Color,
		x,
		y,
		Font,
		Scale,
		true
	);
}
