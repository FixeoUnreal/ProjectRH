// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "LobbyGameMode.h"
#include <Engine/World.h>
#include "TimerManager.h"
#include <OnlineSessionInterface.h>
#include "RHGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;
	if (PlayerCount >= 2)
	{
		FTimerHandle TimerHandle_StartSession;
		GetWorldTimerManager().SetTimer(TimerHandle_StartSession, this, &ALobbyGameMode::StartGame, 60.f);

	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerCount--;
}

void ALobbyGameMode::StartGame()
{
	// Stop players from joining
	URHGameInstance* GameInstance = Cast<URHGameInstance>(GetGameInstance());
	if (!ensure(GameInstance)) { return; }
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Test_Map?listen");
}



