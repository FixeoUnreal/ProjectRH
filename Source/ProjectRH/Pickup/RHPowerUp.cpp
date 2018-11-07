// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "ProjectRH/Pickup/RHPowerUp.h"
#include "TimerManager.h"
#include <UnrealNetwork.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PointLightComponent.h>
#include <GameFramework/RotatingMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Components/SkeletalMeshComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include "Particles/ParticleSystem.h"
#include "Character/ProjectRHCharacter.h"

// Sets default values
ARHPowerUp::ARHPowerUp()
{
	// Initializes component
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComp"));
	PointLightComp->SetCastShadows(false);
	PointLightComp->SetupAttachment(MeshComp);

	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComp"));

	// Initializes primitive variables
	bIsPowerupAcquired = false;

	// Setup multiplayer 
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ARHPowerUp::BeginPlay()
{
	Super::BeginPlay();

}


void ARHPowerUp::OnRep_PowerupAcquired()
{
	OnPowerupStateChanged(bIsPowerupAcquired);

	if (AcquireEffect)
	{
		AcquireEffectComp = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			AcquireEffect,
			GetActorTransform()
		);
		FTimerHandle TimerHandle_FinishEffect;
		GetWorldTimerManager().SetTimer(TimerHandle_FinishEffect, this, &ARHPowerUp::EndAcquireEffect, 2.f);
	}
	if (SoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundEffect, GetActorLocation());
	}
	MeshComp->SetVisibility(false, true);
}

void ARHPowerUp::MulticastOnActivate_Implementation()
{
	OnActivate();
}

void ARHPowerUp::EndAcquireEffect()
{
	if (AcquireEffectComp)
	{
		AcquireEffectComp->Deactivate();
	}
}

void ARHPowerUp::AcquirePowerup(AActor* ActivateFor)
{
	if (Role == ROLE_Authority)
	{
		//Deliver the character the corresponding PowerUp
		if (!ActivateFor) { return; }
		AProjectRHCharacter* PlayerChar = Cast<AProjectRHCharacter>(ActivateFor);
		if (!PlayerChar) { return; }
		PlayerChar->SetPowerUp(this);
		OwningCharacter = PlayerChar;
	}


	bIsPowerupAcquired = true;
	OnRep_PowerupAcquired();

}

void ARHPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARHPowerUp, bIsPowerupAcquired);
	DOREPLIFETIME(ARHPowerUp, OwningCharacter);
}

void ARHPowerUp::Activate()
{
	if (OwningCharacter)
	{
		MulticastOnActivate();
	}
}

