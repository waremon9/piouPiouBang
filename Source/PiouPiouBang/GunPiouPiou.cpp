// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPiouPiou.h"
#include "PiouPiouBangCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Cactus.h"

AGunPiouPiou::AGunPiouPiou()
{

}

void AGunPiouPiou::Shoot()
{
	if (Cooldown > 0 || QteBullet <= 0) return;
	Super::Shoot();

	UWorld* const World = GetWorld();
	const APlayerController* PlayerController = World->GetFirstPlayerController();
	const APiouPiouBangCharacter* PlayerCharacter = (APiouPiouBangCharacter*)PlayerController->GetCharacter();

	const FRotator SpawnRotation = PlayerController->GetControlRotation();

	const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();


	//ray tracing
	FHitResult hit;

	UCameraComponent* Camera = PlayerCharacter->GetFirstPersonCameraComponent();

	FVector oStartPosition = Camera->GetComponentLocation() + Camera->GetForwardVector() * 100;
	FVector oEndPosition = oStartPosition + Camera->GetForwardVector() * 10000;

	if (World->LineTraceSingleByChannel(hit, oStartPosition, oEndPosition, ECollisionChannel::ECC_Visibility))
	{

		//hit particle
		UGameplayStatics::SpawnEmitterAtLocation(World, hitParticle, hit.Location, SpawnRotation, true);

		FVector Knockback = SpawnRotation.RotateVector(FVector::ForwardVector);

		//apply force
		if (hit.GetActor()->IsRootComponentMovable()) {
			UStaticMeshComponent* MeshRootComp = Cast<UStaticMeshComponent>(hit.GetActor()->GetRootComponent());

			if (MeshRootComp != nullptr) MeshRootComp->AddForce(Knockback * 400000 * MeshRootComp->GetMass());
		}

		if (hit.GetActor()->GetName().Contains("BP_Cactus")) {
			((ACactus*)hit.GetActor())->Damage(Damage, FVector(Knockback.X, Knockback.Y, Knockback.Z + 0.45));
		}

	}

	//shoot particle
	UGameplayStatics::SpawnEmitterAtLocation(World, shootParticle, SpawnLocation, SpawnRotation, true);

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = PlayerCharacter->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
