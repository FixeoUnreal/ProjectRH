// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "FinishLapZone.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/Controller.h>
#include "Replication/RHPlayerState.h"
#include "Character/ProjectRHCharacter.h"
#include "Replication/RHGameState.h"
#include <Kismet/GameplayStatics.h>

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
		if (!OverlappingCharacter) { return; }
		AController* CharacterController = OverlappingCharacter->GetController();
		if (!ensure(CharacterController)) { return; }
		ARHPlayerState* CharacterPlayerState = Cast<ARHPlayerState>(CharacterController->PlayerState);
		if (!ensure(CharacterPlayerState)) { return; }

		// Already finished the race
		if(CharacterPlayerState->GetFinalPosition() != ARHPlayerState::FinalPosition_None){ return; }

		int32 NewLapCount = CharacterPlayerState->GetLapCount() + 1;
		CharacterPlayerState->SetLapCount(NewLapCount);

		if (CharacterPlayerState->GetLapCount() >= MaxLapNumber)
		{
			// Finish the race for the overlapping player
			ARHGameState* RHGameState = Cast<ARHGameState>(UGameplayStatics::GetGameState(this));
			if(!ensure(RHGameState)){ return; }
			TArray<ARHPlayerState*> PositionList = RHGameState->GetPositionList();
			if (PositionList.Num() > 0)
			{
				int32 CurrentPosition = PositionList.Find(CharacterPlayerState);
				if (CurrentPosition != INDEX_NONE)
				{
					CharacterPlayerState->SetFinalPosition(CurrentPosition);
				}
			}
			
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
