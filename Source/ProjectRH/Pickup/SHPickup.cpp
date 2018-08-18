// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "ProjectRH/Pickup/SHPickup.h"
#include <Components/SphereComponent.h>
#include <Components/DecalComponent.h>
#include <Engine/World.h>
#include "TimerManager.h"
#include "SHPowerUp.h"

// Sets default values
ASHPickup::ASHPickup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.f);
	SetRootComponent(SphereComp);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	DecalComp->DecalSize = FVector(64.f, 75.f, 75.f);
	DecalComp->SetupAttachment(RootComponent);

	CooldownDuration = 10.f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASHPickup::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		Respawn();
	}
}

void ASHPickup::Respawn()
{
	if (!PowerupClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s. Please update your BP"), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerupInstance = GetWorld()->SpawnActor<ASHPowerUp>(PowerupClass, GetTransform(), SpawnParams);

}

void ASHPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority && PowerupInstance)
	{
		PowerupInstance->AcquirePowerup(OtherActor);
		PowerupInstance = nullptr;

		// Set timer to respawn
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASHPickup::Respawn, CooldownDuration);

	}
}

