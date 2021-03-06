// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RHPickup.generated.h"

class USphereComponent;
class UDecalComponent;
class ASPowerupActor;
class ARHPowerUp;

UCLASS()
class PROJECTRH_API ARHPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARHPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere, Category = "PickupActor")
	TSubclassOf<ARHPowerUp> PowerupClass;

	ARHPowerUp* PowerupInstance;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	float CooldownDuration;

	FTimerHandle TimerHandle_RespawnTimer;
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	
};
