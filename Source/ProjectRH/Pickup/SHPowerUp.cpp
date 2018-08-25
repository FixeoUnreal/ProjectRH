// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "ProjectRH/Pickup/SHPowerUp.h"
#include "TimerManager.h"
#include <UnrealNetwork.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PointLightComponent.h>
#include <GameFramework/RotatingMovementComponent.h>
#include "ProjectRHCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Components/SkeletalMeshComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include "Particles/ParticleSystem.h"

// Sets default values
ASHPowerUp::ASHPowerUp()
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
	bIsPowerupActive = false;

	// Setup multiplayer 
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASHPowerUp::BeginPlay()
{
	Super::BeginPlay();

}


void ASHPowerUp::OnRep_PowerupAcquired()
{
	OnPowerupStateChanged(bIsPowerupActive);

	if (AcquireEffect)
	{
		AcquireEffectComp = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			AcquireEffect,
			GetActorTransform()
		);
		FTimerHandle TimerHandle_FinishEffect;
		GetWorldTimerManager().SetTimer(TimerHandle_FinishEffect, this, &ASHPowerUp::EndAcquireEffect, 2.f);
	}
	MeshComp->SetVisibility(false, true);
}

void ASHPowerUp::EndAcquireEffect()
{
	if (AcquireEffectComp)
	{
		AcquireEffectComp->Deactivate();
	}
}

void ASHPowerUp::AcquirePowerup(AActor* ActivateFor)
{
	if (Role == ROLE_Authority)
	{
		//Deliver the character the corresponding PowerUp
		if (!ActivateFor) { return; }
		AProjectRHCharacter* PlayerChar = Cast<AProjectRHCharacter>(ActivateFor);
		if (!PlayerChar) { return; }
		PlayerChar->SetPowerUp(this);
	}


	bIsPowerupActive = true;
	OnRep_PowerupAcquired();

}

void ASHPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASHPowerUp, bIsPowerupActive);
}

void ASHPowerUp::Activate(AProjectRHCharacter* PlayerChar)
{
	// TODO Implement logic before and after OnActivate call if needed

	// Logic that will be implemented in subclass BP
	OnActivate(PlayerChar);
}

