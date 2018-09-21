// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "FinishLapZone.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/Controller.h>
#include "RHPlayerState.h"
#include "ProjectRHCharacter.h"

// Sets default values
AFinishLapZone::AFinishLapZone()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("OverlapAllDynamic");
	SetRootComponent(MeshComp);
}

void AFinishLapZone::OnMeshCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (HasAuthority())
	{
		AProjectRHCharacter* OverlappingCharacter = Cast<AProjectRHCharacter>(OtherActor);
		if (!ensure(OverlappingCharacter)) { return; }
		AController* CharacterController = OverlappingCharacter->GetController();
		if (!ensure(CharacterController)) { return; }
		ARHPlayerState* CharacterPlayerState = Cast<ARHPlayerState>(CharacterController->PlayerState);
		if (!ensure(CharacterPlayerState)) { return; }

		int32 NewLapCount = CharacterPlayerState->GetLapCount() + 1;
		CharacterPlayerState->SetLapCount(NewLapCount);

		if (CharacterPlayerState->GetLapCount() >= MaxLapNumber)
		{
			// Finish the race for the overlapping player
			OverlappingCharacter->bInRunMode = false;
		}
	}
}

// Called when the game starts or when spawned
void AFinishLapZone::BeginPlay()
{
	Super::BeginPlay();
	
	if(!ensure(MeshComp)){ return; }
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AFinishLapZone::OnMeshCompBeginOverlap);
}
