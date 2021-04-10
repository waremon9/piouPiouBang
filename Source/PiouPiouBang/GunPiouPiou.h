// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GunPiouPiou.generated.h"

/**
 * 
 */
UCLASS()
class PIOUPIOUBANG_API AGunPiouPiou : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Particle)
		UParticleSystem* hitParticle;
	
public:
	AGunPiouPiou();

	void Shoot() override;
};
