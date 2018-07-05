// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectRHGameMode.h"
#include "ProjectRHCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectRHGameMode::AProjectRHGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Game_Character/Behaviour/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
