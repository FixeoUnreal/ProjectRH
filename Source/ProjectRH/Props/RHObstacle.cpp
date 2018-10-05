// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHObstacle.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Components/SceneComponent.h>
#include "Character/ProjectRHCharacter.h"


// Sets default values
ARHObstacle::ARHObstacle()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(MeshComp);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ARHObstacle::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARHObstacle::OnBoxCollisionBeginOverlap);
}

void ARHObstacle::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	OverlappingCharacter = Cast<AProjectRHCharacter>(OtherActor);

	if (HasAuthority() && OverlappingCharacter)
	{
		ApplyCollisionEffect();

		MulticastSetVisibility();
	}
}

void ARHObstacle::MulticastSetVisibility_Implementation()
{
	if (bInvisibleAfterOverlap)
	{
		// Make obstacle unimpactable after collision
		UE_LOG(LogTemp, Warning, TEXT("Obstacle: Set Vis"));
		MeshComp->SetVisibility(false, true);
		MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

