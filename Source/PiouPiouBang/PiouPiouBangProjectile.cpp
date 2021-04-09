// Copyright Epic; Games, Inc. All Rights Reserved.

#include "PiouPiouBangProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Cactus.h"
#include "PiouPiouBangGameMode.h"

APiouPiouBangProjectile::APiouPiouBangProjectile() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APiouPiouBangProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void APiouPiouBangProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor->GetName().Contains("BP_Cactus"))
	{
		Explosion();
	}
}

void APiouPiouBangProjectile::Tick(float dt) {
	BombTimer -= dt;

	if (BombTimer<=0) Explosion();
}

void APiouPiouBangProjectile::Explosion() {

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticle, GetActorLocation(), GetActorRotation(), true);

	APiouPiouBangGameMode* GameMode = (APiouPiouBangGameMode*)GetWorld()->GetAuthGameMode();
	TArray<ACactus*> MesCactus = GameMode->GetAllCactus();

	for (ACactus* cac : MesCactus) {
		float Distance = FVector::Distance(cac->GetActorLocation(), GetActorLocation());
		if ( Distance <= ExplosionRange) {

			FRotator Rotation = (cac->GetActorLocation() - GetActorLocation()).Rotation();
			FVector Knockback = Rotation.RotateVector(FVector::ForwardVector);
			Knockback.Normalize();

			cac->Damage(10, FVector(Knockback.X, Knockback.Y, Knockback.Z + 0.6));
		}
	}

	Destroy();
}