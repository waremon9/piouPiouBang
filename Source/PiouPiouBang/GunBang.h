// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GunBang.generated.h"

/**
 * 
 */
UCLASS()
class PIOUPIOUBANG_API AGunBang : public AWeapon
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class APiouPiouBangProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		float BombTimer;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		float ExplosionRange;

public:
	AGunBang();

	void Shoot() override;
};
