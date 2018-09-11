// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuInGame.h"
#include "Components/Button.h"
#include <Kismet/KismetSystemLibrary.h>
#include "MenuInterface.h"


void UMenuInGame::Setup()
{
	this->AddToViewport();

	// Set input mode
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeGameAndUI.SetWidgetToFocus(this->TakeWidget());

	PC->SetInputMode(InputModeGameAndUI);
	PC->bShowMouseCursor = true;
}

bool UMenuInGame::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	// Bind onclick events
	if (!ensure(BtnResume)) { return false; }
	BtnResume->OnClicked.AddDynamic(this, &UMenuInGame::ResumeClicked);
	if (!ensure(BtnQuit)) { return false; }
	BtnQuit->OnClicked.AddDynamic(this, &UMenuInGame::QuitClicked);
	if (!ensure(BtnMainMenu)) { return false; }
	BtnMainMenu->OnClicked.AddDynamic(this, &UMenuInGame::MainMenuClicked);

	UE_LOG(LogTemp, Warning, TEXT("Initilize called"));
	return true;
}

void UMenuInGame::ResumeClicked()
{
	TearDown();
}

void UMenuInGame::QuitClicked()
{
	UWorld* Wolrd = GetWorld();
	if (!ensure(Wolrd)) { return; }
	APlayerController* PC = Wolrd->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit);
}

void UMenuInGame::MainMenuClicked()
{
	if(!ensure(MenuInterface)){ return; }
	TearDown();
	MenuInterface->OpenMainMenu();
}
