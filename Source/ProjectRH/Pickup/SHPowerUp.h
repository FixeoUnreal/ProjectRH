// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHPowerUp.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class URotatingMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;
class AProjectRHCharacter;
class UTexture2D;

UCLASS()
class PROJECTRH_API ASHPowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHPowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PowerupAcquired();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChanged(bool bNewIsActive);

	// Implemented in subclass and determines powerup's behaviour on activation
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivate();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnActivate();

	void EndAcquireEffect();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Powerups")
	AProjectRHCharacter* OwningCharacter;

	UPROPERTY(ReplicatedUsing = OnRep_PowerupAcquired)
	bool bIsPowerupAcquired;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPointLightComponent* PointLightComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URotatingMovementComponent* RotatingMovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UParticleSystem* AcquireEffect;

	UParticleSystemComponent* AcquireEffectComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Powerup")
	UTexture2D* PowerupUIImage;

public:
	void AcquirePowerup(AActor* ActivateFor);

	// Function to call after acquiring powerup (Play animations, particle systems, etc.)
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnAcquired(AActor* ActivateFor);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Activate Powerup, which can only be called on server
	void Activate();
	
};
