// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHPlayerController.h"
#include <UnrealNetwork.h>




void ARHPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARHPlayerController, bInLobby);
}
