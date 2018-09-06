// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHObstacle.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include "ProjectRHCharacter.h"


// Sets default values
ARHObstacle::ARHObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ARHObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void ARHObstacle::MulticastBeginOverlap_Implementation()
{
	OnBeginOverlap(OverlappingCharacter);
}


// Called every frame
void ARHObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARHObstacle::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	OverlappingCharacter = Cast<AProjectRHCharacter>(OtherActor);

	if (HasAuthority() && OverlappingCharacter)
	{
		MulticastBeginOverlap();
	}

	// Make obstacle unimpactable after collision
	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
}

