// Fill out your copyright notice in the Description page of Project Settings.


#include "Cactus.h"

// Sets default values
ACactus::ACactus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshCactus = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");

	CBonTuPeDepop = false;
	WeshTMor = false;
	Speed = 500;
}

// Called when the game starts or when spawned
void ACactus::BeginPlay()
{
	Super::BeginPlay();
	
	if (CactusRun != nullptr)
	{
		bool bLoop = true;
		MeshCactus->PlayAnimation(CactusRun, bLoop);
	}
}

// Called every frame
void ACactus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!WeshTMor) FollowPlayer(DeltaTime);
}

bool ACactus::GetBoolDespawn()
{
	return CBonTuPeDepop;
}

void ACactus::Damage(int dmg) {
	//pas de vie pour l'instant.
	//instant death
	Death();
}

void ACactus::Death() {
	/*if (CactusDeath != nullptr)
	{
		bool bLoop = false;
		MeshCactus->PlayAnimation(CactusDeath, bLoop);
	}*/

	MeshCactus->SetSimulatePhysics(true);

	SetActorEnableCollision(false);
	WeshTMor = true;

	GetWorld()->GetTimerManager().SetTimer(DespawnTimer, this, &ACactus::Despawn, 2.5, false);
}

void ACactus::FollowPlayer(float dt)
{

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector Vector = PlayerLocation - GetActorLocation();
	Vector.Normalize();
	float radian = atan2(Vector.Y, Vector.X);
	float degree = radian * 180.f / PI - 90;

	SetActorRotation(FRotator(0, degree, 0) );
	UE_LOG(LogTemp, Log, TEXT("%f"), degree);

	SetActorLocation(GetActorLocation() + GetActorRightVector() * Speed * dt);

}

void ACactus::Despawn() {
	CBonTuPeDepop = true;
}