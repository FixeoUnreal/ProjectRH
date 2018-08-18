// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "ProjectRH/Pickup/SHPowerUp.h"
#include "TimerManager.h"
#include <UnrealNetwork.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PointLightComponent.h>
#include <GameFramework/RotatingMovementComponent.h>
#include "ProjectRHCharacter.h"

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


void ASHPowerUp::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);

	MeshComp->SetVisibility(false, true);
}

void ASHPowerUp::AcquirePowerup(AActor* ActivateFor)
{
	//OnAcquired(ActivateFor);

	//Deliver the character the corresponding PowerUp
	if(!ActivateFor){ return; }
	AProjectRHCharacter* PlayerChar = Cast<AProjectRHCharacter>(ActivateFor);
	if(!PlayerChar){ return; }
	PlayerChar->SetPowerUp(this);

	bIsPowerupActive = true;
	OnRep_PowerupActive();

}

void ASHPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASHPowerUp, bIsPowerupActive);
}


