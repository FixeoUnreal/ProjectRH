// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "WayGate.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/Controller.h>
#include <GameFramework/Character.h>
#include "RHPlayerState.h"


// Sets default values
AWayGate::AWayGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("OverlapAllDynamic");
	SetRootComponent(MeshComp);

}

// Called when the game starts or when spawned
void AWayGate::BeginPlay()
{
	Super::BeginPlay();
	
	if(!ensure(MeshComp)){ return; }
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AWayGate::OnMeshCompBeginOverlap);
}

void AWayGate::OnMeshCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (HasAuthority())
	{
		ACharacter* OverlappingCharacter = Cast<ACharacter>(OtherActor);
		if(!OverlappingCharacter){ return; }
		AController* CharacterController = OverlappingCharacter->GetController();
		if(!ensure(CharacterController)){ return; }
		ARHPlayerState* CharacterPlayerState = Cast<ARHPlayerState>(CharacterController->PlayerState);
		if(!CharacterPlayerState){ return; }

		// Increases this count for player's position measurement
		int32 NewCount = CharacterPlayerState->GetPassedWayGateCount() + 1;
		CharacterPlayerState->SetPassedWayGateCount(NewCount);
		CharacterPlayerState->UpdateNextWayGate();
	}
	
}

// Called every frame
void AWayGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

