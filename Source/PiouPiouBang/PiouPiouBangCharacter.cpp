// Copyright Epic Games, Inc. All Rights Reserved.

#include "PiouPiouBangCharacter.h"
#include "PiouPiouBangProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include <Kismet/GameplayStatics.h>
#include <DrawDebugHelpers.h>
#include "UObject/ConstructorHelpers.h"

#include "Cactus.h"
#include "GunPiouPiou.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APiouPiouBangCharacter

APiouPiouBangCharacter::APiouPiouBangCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GunSelected = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	static ConstructorHelpers::FObjectFinder<UClass> GunClassFinder(TEXT("Blueprint'/Game/Blueprints/MyGunPiouPiou.MyGunPiouPiou_C'"));
	GunPiouPiou = GunClassFinder.Object;

	GunOffset = FVector(80.0f, 20.0f, 100.0f);

	Shooting = false;
}

void APiouPiouBangCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PiouPiou = GetWorld()->SpawnActor<AGunPiouPiou>(GunPiouPiou, GetActorLocation(), FRotator::ZeroRotator);
	PiouPiou->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void APiouPiouBangCharacter::Tick(float dt) {

	if (Shooting) {
		if (GunSelected)
		{
			PiouPiou->Shoot();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("BangPan"));
		}
	}
}

void APiouPiouBangCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APiouPiouBangCharacter::OnFirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APiouPiouBangCharacter::OnFireReleased);

	// Bind switch weapon event
	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &APiouPiouBangCharacter::OnSwitch);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APiouPiouBangCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APiouPiouBangCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APiouPiouBangCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APiouPiouBangCharacter::LookUpAtRate);
}

void APiouPiouBangCharacter::OnFirePressed()
{
	Shooting = true;
}

void APiouPiouBangCharacter::OnFireReleased()
{
	Shooting = false;
}

/*void APiouPiouBangCharacter::OnFireBang()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<APiouPiouBangProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}*/

void APiouPiouBangCharacter::OnSwitch()
{
	if (GunSelected) {
		UE_LOG(LogTemp, Log, TEXT("BangSwap"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PiouPiouSwap"));
	}
	GunSelected = !GunSelected;
}


void APiouPiouBangCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APiouPiouBangCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APiouPiouBangCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APiouPiouBangCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
