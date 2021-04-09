// Fill out your copyright notice in the Description page of Project Settings.


#include "Cactus.h"
#include "PiouPiouBangGameMode.h"

// Sets default values
ACactus::ACactus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshCactus = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");

	CBonTuPeDepop = false;
	WeshTMor = false;
	IsAttacking = false;
}

// Called when the game starts or when spawned
void ACactus::BeginPlay()
{
	Super::BeginPlay();
	MeshCactus->PlayAnimation(CactusRun, true);
}

// Called every frame
void ACactus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!WeshTMor) 
	{
		if (IsAttacking) {
			if (!MeshCactus->IsPlaying()) {
				IsAttacking = false;
				MeshCactus->PlayAnimation(CactusRun, true);
			}
		}
		else {
			if (FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), GetActorLocation()) < 150)
			{
				MeshCactus->PlayAnimation(CactusAttack, false);
				IsAttacking = true;
			}
			else
			{
				FollowPlayer(DeltaTime);
			}
		}
	}
}

bool ACactus::GetBoolDespawn()
{
	return CBonTuPeDepop;
}

void ACactus::Damage(int dmg, FVector hitDirection) {
	//pas de vie pour l'instant.
	//instant death
	Death(hitDirection);
}

void ACactus::Death(FVector hitDirection) {

	MeshCactus->SetSimulatePhysics(true);

	//SetActorEnableCollision(false);
	MeshCactus->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	MeshCactus->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	WeshTMor = true;

	MeshCactus->AddForce(hitDirection * 230000 * MeshCactus->GetMass());

	GetWorld()->GetTimerManager().SetTimer(DespawnTimer, this, &ACactus::Despawn, 2.5, false);

	APiouPiouBangGameMode* GameMode = (APiouPiouBangGameMode*)GetWorld()->GetAuthGameMode();
	GameMode->AddScore(100);
}

void ACactus::FollowPlayer(float dt)
{

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector Vector = PlayerLocation - GetActorLocation();
	Vector.Normalize();
	float radian = atan2(Vector.Y, Vector.X);
	float degree = radian * 180.f / PI - 90;

	SetActorRotation(FRotator(0, degree, 0) );

	SetActorLocation(GetActorLocation() + GetActorRightVector() * Speed * dt);

}

void ACactus::Despawn() {
	CBonTuPeDepop = true;
}