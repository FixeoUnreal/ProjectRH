// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHGameMode.h"
#include "Replication/RHPlayerState.h"
#include "Replication/RHGameState.h"

void ARHGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Adds new player to score system
	// This is for rooms that are hosted by players, because the InitializeScoreboard from GameState
	// is called before PostLogin of joining players
	ARHGameState* RHGameState = Cast<ARHGameState>(GameState);
	if(!ensure(RHGameState)){ return; }
	ARHPlayerState* RHPlayerState = Cast<ARHPlayerState>(NewPlayer->PlayerState);
	if(!ensure(RHPlayerState)){ return; }
	UE_LOG(LogTemp, Warning, TEXT("Post login"));
	RHGameState->AddNewPlayerToScoreBoard(RHPlayerState);
}

