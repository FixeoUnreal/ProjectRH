// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHLaunchPad.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ARHLaunchPad::ARHLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component's initialization
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	BaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComp"));
	BaseComp->SetupAttachment(RootComponent);
	BaseComp->SetGenerateOverlapEvents(false);

	SurfaceComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SurfaceComp"));
	SurfaceComp->SetupAttachment(BaseComp);
	SurfaceComp->SetGenerateOverlapEvents(false);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(SurfaceComp);

	// Value's initialization
	UpwardsForce = 1500.f;
}

// Called when the game starts or when spawned
void ARHLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind functions to events
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARHLaunchPad::LaunchOverlappingActor);
}

void ARHLaunchPad::LaunchOverlappingActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FVector ForwardForceVector = BoxComp->GetForwardVector() * 500;
	FVector LaunchVelocity = ForwardForceVector + FVector(0, 0, UpwardsForce);
	ACharacter* OverlappingActor = Cast<ACharacter>(OtherActor);

	if (OverlappingActor)
	{
		OverlappingActor->LaunchCharacter(LaunchVelocity, false, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		// Reset current velocity of the object
		OtherComp->SetAllPhysicsLinearVelocity(FVector(0));

		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchEffect, GetActorLocation());
	}
}

// Called every frame
void ARHLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

