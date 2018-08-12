// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RHLaunchPad.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystem;

UCLASS()
class PROJECTRH_API ARHLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARHLaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void LaunchOverlappingActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent * Scene;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BaseComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* SurfaceComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* LaunchEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects", meta = (ClampMin = "0.0",ClampMax = "5000.0"))
	float UpwardsForce;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
