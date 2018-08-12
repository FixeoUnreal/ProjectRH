// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "Floor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>


// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component's initialization
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->RelativeLocation = FVector(2000, 0, 0);
	MeshComp->SetupAttachment(RootComponent);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->RelativeLocation = FVector(4000, 0, 0);
	ArrowComp->RelativeRotation = FRotator(5, 0, 0);
	ArrowComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


