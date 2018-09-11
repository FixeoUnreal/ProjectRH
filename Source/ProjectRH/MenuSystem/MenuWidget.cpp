// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"
#include "MenuInterface.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();

	// Set input mode
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	FInputModeUIOnly InputModeUI;
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeUI.SetWidgetToFocus(this->TakeWidget());

	PC->SetInputMode(InputModeUI);
	PC->bShowMouseCursor = true;
}


void UMenuWidget::TearDown()
{
	FInputModeGameOnly InputModeGameOnly;

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	PC->SetInputMode(InputModeGameOnly);
	PC->bShowMouseCursor = false;

	this->RemoveFromViewport();
}
