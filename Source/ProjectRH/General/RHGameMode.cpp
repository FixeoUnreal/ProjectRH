// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHGameMode.h"
#include "RHGameState.h"
#include "RHPlayerState.h"

void ARHGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Adding new player to score system
	ARHGameState* RHGameState = Cast<ARHGameState>(GameState);
	if(!ensure(RHGameState)){ return; }
	ARHPlayerState* RHPlayerState = Cast<ARHPlayerState>(NewPlayer->PlayerState);
	if(!ensure(RHPlayerState)){ return; }

	RHGameState->AddNewPlayerToScoreBoard(RHPlayerState);
}
