// Fill out your copyright notice in the Description page of Project Settings.


#include "Cactus.h"
#include "PiouPiouBangGameMode.h"
#include "PiouPiouBangCharacter.h"

// Sets default values
ACactus::ACactus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshCactus = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SetRootComponent(MeshCactus);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	DamageCollision->SetupAttachment(MeshCactus);
	DamageCollision->SetRelativeLocation(FVector(0, 50, 90));


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
				Attack();
			}
			else
			{
				FollowPlayer(DeltaTime);
			}
		}
	}
}

void ACactus::Attack() {
	MeshCactus->PlayAnimation(CactusAttack, false);
	IsAttacking = true;
	
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACactus::OnBeginOverlap);
}

void ACactus::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("FirstPersonCharacter")) {
		((APiouPiouBangCharacter*)OtherActor)->GetHit(GetActorLocation());
	}
}

bool ACactus::GetBoolDespawn()
{
	return CBonTuPeDepop;
}

void ACactus::Damage(int dmg, FVector hitDirection) {
	Life -= dmg;
	if (Life <= 0) {

		Death(hitDirection);

	}
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