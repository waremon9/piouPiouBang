// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshWeapon"));
	MeshWeapon->bCastDynamicShadow = false;
	MeshWeapon->CastShadow = false;
	MeshWeapon->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(MeshWeapon);

	Cooldown = 0;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::ResetCooldown()
{
	Cooldown = BaseCooldown;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Cooldown -= DeltaTime;

}

void AWeapon::Shoot()
{
	if (Cooldown > 0 || QteBullet <= 0) return;

	QteBullet--;
	ResetCooldown();
}

