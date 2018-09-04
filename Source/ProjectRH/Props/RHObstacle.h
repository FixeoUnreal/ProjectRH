// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RHObstacle.generated.h"

class UStaticMeshComponent;
class UParticleSystem;
class UBoxComponent;

UCLASS()
class PROJECTRH_API ARHObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARHObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayDestroyEffect();

	void PlayDestroyEffect();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, Category = "RHObstacle")
	UParticleSystem* DestroyEffect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
