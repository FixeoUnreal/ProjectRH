// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHGameInstance.h"
#include <Engine/Engine.h>
#include <GameFramework/PlayerController.h>
#include <ConstructorHelpers.h>
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuWidget.h"


const static FName SESSION_NAME = TEXT("My Session Game");

URHGameInstance::URHGameInstance(const FObjectInitializer & ObjectInitializer)
{
	// Find BP classes if needed
}

void URHGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init..."));
}

void URHGameInstance::OpenMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }
	PC->ClientTravel("/Game/Levels/MainMenu_Map", ETravelType::TRAVEL_Absolute);
}

void URHGameInstance::LoadMenuWidget(TSubclassOf<UMenuWidget> MenuClass)
{
	if (!ensure(MenuClass)) { return; }
	CurrentMenu = CreateWidget<UMenuWidget>(this, MenuClass);
	if (!ensure(CurrentMenu)) { return; }

	CurrentMenu->Setup();

	CurrentMenu->SetMenuInterface(this);
}

void URHGameInstance::ExitMenu()
{
	if (!ensure(CurrentMenu)) { return; }
	CurrentMenu->TearDown();
}


void URHGameInstance::Host()
{
	UE_LOG(LogTemp, Warning, TEXT("Enter Start Session"));
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, TEXT("Hosting "));

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	World->ServerTravel("/Game/Levels/Test_Map?listen");

}

void URHGameInstance::Join()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	const FString& Address = "192.168.0.10";
	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}



