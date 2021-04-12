// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBang.h"
#include "PiouPiouBangCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Cactus.h"
#include "PiouPiouBangProjectile.h"
#include "Kismet/GameplayStatics.h"

AGunBang::AGunBang()
{

}

void AGunBang::Shoot()
{
	if (Cooldown > 0 || QteBullet <= 0) return;
	Super::Shoot();


	UWorld* const World = GetWorld();
	const APlayerController* PlayerController = World->GetFirstPlayerController();
	const APiouPiouBangCharacter* PlayerCharacter = (APiouPiouBangCharacter*)PlayerController->GetCharacter();

	const FRotator SpawnRotation = PlayerController->GetControlRotation();

	const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// spawn the projectile at the muzzle
	APiouPiouBangProjectile* bullet = World->SpawnActor<APiouPiouBangProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	if (bullet != nullptr) {
		bullet->SetDamage(Damage);
		bullet->SetExplosionRange(ExplosionRange);
		bullet->SetBombTimer(BombTimer);
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
