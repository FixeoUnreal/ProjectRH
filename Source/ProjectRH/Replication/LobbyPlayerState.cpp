// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "LobbyPlayerState.h"
#include <UnrealNetwork.h>


void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
	DOREPLIFETIME(ALobbyPlayerState, bIsHost);
}

