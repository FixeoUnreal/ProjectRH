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

	UFUNCTION()
	void OnRep_PowerupActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChanged(bool bNewIsActive);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivate(ACharacter* PlayerChar);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnActivate(ACharacter* PlayerChar);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void PlayActivationEffects();

	void EndAcquireEffect();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Powerups")
	AProjectRHCharacter* OwningCharacter;

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActivated)
	bool bIsPowerupActivated;

	UPROPERTY(ReplicatedUsing = OnRep_PowerupAcquired)
	bool bIsPowerupActive;

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

public:
	void AcquirePowerup(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnAcquired(AActor* ActivateFor);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Activate(ACharacter* PlayerChar);
	
};
