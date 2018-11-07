// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHGameState.h"
#include "RHPlayerState.h"
#include <UnrealNetwork.h>
#include "Terrain/WayGate.h"
#include <Kismet/GameplayStatics.h>


ARHGameState::ARHGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;
}


void ARHGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		InitializeScoreBoard();
	}
}

void ARHGameState::InitializeScoreBoard()
{
	// Finds all WayGates and puts them in a sorted list
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, AWayGate::StaticClass(), FoundActors);
	for (AActor* FoundActor : FoundActors)
	{
		AWayGate* WayGate = Cast<AWayGate>(FoundActor);
		if (WayGate)
		{
			WayGateList.Add(WayGate);
		}
	}

	WayGateList.Sort([](const AWayGate& GateA, const AWayGate& GateB) {
		return GateA.Index < GateB.Index;
	});
	if (WayGateList.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WayGate Size: %d"), WayGateList.Num());
		UE_LOG(LogTemp, Warning, TEXT("PlayerArr Size: %d"), PlayerArray.Num());
	}

	// Populates PlayerStates with array of WayGates in numerical order
	// and populates score board
	for (APlayerState* PlayerState : PlayerArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player state name: %s , Inactive: %d"), *PlayerState->GetName(), PlayerState->bIsInactive);
		ARHPlayerState* RHPlayerState = Cast<ARHPlayerState>(PlayerState);
		
		AddNewPlayerToScoreBoard(RHPlayerState);
	}
}

void ARHGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() && HasMatchStarted())
	{
			UpdateScoreBoard();
	}
}


void ARHGameState::UpdateScoreBoard()
{
	if (PositionList.Num() > 0)
	{
		// Sorts positions based on number of laps, passed WayGates and distances to next WayGate
		PositionList.Sort([](const ARHPlayerState& PlayerStateA, const ARHPlayerState& PlayerStateB) {
			if (PlayerStateA.GetFinalPosition() != PlayerStateB.GetFinalPosition())
			{
				return PlayerStateA.GetFinalPosition() < PlayerStateB.GetFinalPosition();
			}
			if (PlayerStateA.GetLapCount() != PlayerStateB.GetLapCount())
			{
				return PlayerStateA.GetLapCount() > PlayerStateB.GetLapCount();
			}
			if (PlayerStateA.GetPassedWayGateCount() != PlayerStateB.GetPassedWayGateCount())
			{
				return PlayerStateA.GetPassedWayGateCount() > PlayerStateB.GetPassedWayGateCount();
			}
			
			return PlayerStateA.GetDistanceToNextWayGate() < PlayerStateB.GetDistanceToNextWayGate();
			
		});
	}
}

void ARHGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARHGameState, PositionList);
}

TArray<ARHPlayerState*> ARHGameState::GetPositionList() const
{
	return PositionList;
}

void ARHGameState::AddNewPlayerToScoreBoard(ARHPlayerState* RHPlayerState)
{
	// Populates PlayerState with array of WayGates in numerical order
	if(!RHPlayerState){ return; }
	UE_LOG(LogTemp, Warning, TEXT("Enter ADD"));

	// Already in list
	if(PositionList.Find(RHPlayerState) != INDEX_NONE){ return; }
	UE_LOG(LogTemp, Warning, TEXT("After check exist in list"));
	if (WayGateList.Num() > 0)
	{
		RHPlayerState->WayGateList = WayGateList;
		PositionList.Add(RHPlayerState);
		RHPlayerState->SetNextWayGate(WayGateList[0]);
		UE_LOG(LogTemp, Warning, TEXT("After ADD: %s"), *WayGateList[0]->GetName());
	}
}
