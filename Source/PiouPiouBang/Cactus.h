// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cactus.generated.h"

UCLASS()
class PIOUPIOUBANG_API ACactus : public AActor
{
	GENERATED_BODY()

	/** Mesh of cactus*/
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MeshCactus;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		float Speed;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		float Life;
	
public:	
	// Sets default values for this actor's properties
	ACactus();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimSequence* CactusIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimSequence* CactusAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimSequence* CactusRun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimSequence* CactusDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetBoolDespawn();

	void Damage(int dmg, FVector hitDirection);
	void Death(FVector hitDirection);

	void FollowPlayer(float dt);

private:
	FTimerHandle DespawnTimer;
	bool CBonTuPeDepop;
	bool WeshTMor;
	bool IsAttacking;

	void Despawn();
};
