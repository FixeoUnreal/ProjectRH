// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHPowerUp.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class URotatingMovementComponent;

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
	void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChanged(bool bNewIsActive);

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
	bool bIsPowerupActive;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPointLightComponent* PointLightComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URotatingMovementComponent* RotatingMovementComp;

public:
	void AcquirePowerup(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnAcquired(AActor* ActivateFor);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void Activate(AProjectRHCharacter* PlayerChar);
	
};
