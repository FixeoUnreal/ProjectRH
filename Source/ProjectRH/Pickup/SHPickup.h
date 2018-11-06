// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHPickup.generated.h"

class USphereComponent;
class UDecalComponent;
class ASPowerupActor;
class ASHPowerUp;

UCLASS()
class PROJECTRH_API ASHPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere, Category = "PickupActor")
	TSubclassOf<ASHPowerUp> PowerupClass;

	ASHPowerUp* PowerupInstance;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	float CooldownDuration;

	FTimerHandle TimerHandle_RespawnTimer;
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	
};
