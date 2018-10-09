// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHPlayerState.h"
#include <UnrealNetwork.h>
#include "Terrain/WayGate.h"



void ARHPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARHPlayerState, PassedWayGateCount);
	DOREPLIFETIME(ARHPlayerState, LapCount);
	DOREPLIFETIME(ARHPlayerState, DistanceToNextWayGate);
	DOREPLIFETIME(ARHPlayerState, FinalPosition);
	DOREPLIFETIME(ARHPlayerState, bIsReady);
}

int32 ARHPlayerState::GetPassedWayGateCount() const
{
	return PassedWayGateCount;
}	

int32 ARHPlayerState::GetLapCount() const
{
	return LapCount;
}

float ARHPlayerState::GetDistanceToNextWayGate() const
{
	return DistanceToNextWayGate;
}

void ARHPlayerState::SetPassedWayGateCount(int32 InPassedWayGateCount)
{
	PassedWayGateCount = InPassedWayGateCount;
}

void ARHPlayerState::SetLapCount(int32 InLapCount)
{
	LapCount = InLapCount;
}

void ARHPlayerState::SetDistanceToNextWayGate(float InDistanceToNextWayGate)
{
	DistanceToNextWayGate = InDistanceToNextWayGate;
}

int32 ARHPlayerState::GetFinalPosition() const
{
	return FinalPosition;
}

void ARHPlayerState::SetFinalPosition(int32 InFinalPosition)
{
	FinalPosition = InFinalPosition;
}

AWayGate* ARHPlayerState::GetNexWayGate() const
{
	return NextWayGate;
}

void ARHPlayerState::SetNextWayGate(AWayGate* InWayGate)
{
	NextWayGate = InWayGate;
}

void ARHPlayerState::UpdateNextWayGate()
{
	if (WayGateList.Num() > 0)
	{
		WayGateIndex = (WayGateIndex + 1) % WayGateList.Num();
		NextWayGate = WayGateList[WayGateIndex];
	}
}
