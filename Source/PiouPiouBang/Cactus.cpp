// Fill out your copyright notice in the Description page of Project Settings.


#include "Cactus.h"

// Sets default values
ACactus::ACactus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshCactus = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");

}

// Called when the game starts or when spawned
void ACactus::BeginPlay()
{
	Super::BeginPlay();
	
	if (CactusAttack != nullptr)
	{
		bool bLoop = true;
		MeshCactus->PlayAnimation(CactusAttack, bLoop);
	}
}

// Called every frame
void ACactus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

