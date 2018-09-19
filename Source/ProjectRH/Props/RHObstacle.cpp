// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHObstacle.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include "ProjectRHCharacter.h"
#include <Components/SceneComponent.h>


// Sets default values
ARHObstacle::ARHObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		MulticastApplyCollisionEffect();
	}
}

void ARHObstacle::MulticastApplyCollisionEffect_Implementation()
{
	ApplyCollisionEffect();

	if (bInvisibleAfterOverlap)
	{
		// Make obstacle unimpactable after collision
		MeshComp->SetVisibility(false, true);
		MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
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

	//OverlappingCharacter = Cast<AProjectRHCharacter>(OtherActor);

	//if (HasAuthority() && OverlappingCharacter)
	//{
	//	MulticastBeginOverlap();
	//}

	
}

