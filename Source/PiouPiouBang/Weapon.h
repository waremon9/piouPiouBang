// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "Weapon.generated.h"

UCLASS()
class PIOUPIOUBANG_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
		UPROPERTY(EditDefaultsOnly, Category = Particle)
			UParticleSystem* shootParticle;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			USkeletalMeshComponent* MeshWeapon;

		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			USceneComponent* FP_MuzzleLocation;

		UPROPERTY(EditAnywhere, Category = Gameplay)
			float Damage;

		UPROPERTY(EditAnywhere, Category = Gameplay)
			float BaseCooldown;

		UPROPERTY(EditAnywhere, Category = Gameplay)
			float QteBullet;

		UPROPERTY(EditAnywhere, Category = Gameplay)
			float BaseReloadCooldown;

		/** Sound to play each time we fire */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			USoundBase* FireSound;

		/** AnimMontage to play each time we fire */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			UAnimMontage* FireAnimation;
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ResetCooldown();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot();

private:
	float Cooldown;
	float ReloadCooldown;
};
