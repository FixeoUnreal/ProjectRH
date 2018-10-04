// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "MenuInterface.h"
#include <WidgetSwitcher.h>
#include <Components/InputComponent.h>
#include <ScrollBox.h>
#include <ConstructorHelpers.h>
#include <TextBlock.h>
#include "MenuSystem/ServerRow.h"
#include <OnlineSessionSettings.h>
#include "EditableTextBox.h"

const FString& SEARCH_WAITING_TEXT = TEXT("Searching for servers...");
const FString& SEARCH_NOT_FOUND_TEXT = TEXT("No server found!");

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class)) { return; }
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	TearDown();
}

void UMainMenu::SetServerList(TArray<FServerData> ServerList)
{
	if (!ensure(ServerScrollBox)) { return; }

	ServerScrollBox->ClearChildren();

	if (ServerList.Num() <= 0)
	{
		InfoMessage->SetText(FText::FromString(SEARCH_NOT_FOUND_TEXT));
		return;
	}

	uint32 i = 0;
	for (CONST FServerData& Data : ServerList)
	{
		InfoMessage->SetText(FText::FromString(""));
		AddServerRow(Data, i++);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::JoinSelectedServer()
{
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index value: %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not set"));
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	// Bind onclick events
	if (!ensure(BtnHost)) { return false; }
	BtnHost->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	if (!ensure(BtnHostWithName)) { return false; }
	BtnHostWithName->OnClicked.AddDynamic(this, &UMainMenu::ConfirmHost);
	if (!ensure(BtnJoin)) { return false; }
	BtnJoin->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	if (!ensure(BtnJoinWithAddr)) { return false; }
	BtnJoinWithAddr->OnClicked.AddDynamic(this, &UMainMenu::ConfirmJoin);
	if (!ensure(BtnBackFromJoin)) { return false; }
	BtnBackFromJoin->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	if (!ensure(BtnBackFromHost)) { return false; }
	BtnBackFromHost->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	if (!ensure(BtnQuit)) { return false; }
	BtnQuit->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	UE_LOG(LogTemp, Warning, TEXT("Initilize called"));
	return true;
}

void UMainMenu::OpenHostMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Host clicked"));
	if (!ensure(MenuWidgetSwitcher)) { return; }
	MenuWidgetSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::ConfirmHost()
{
	if (MenuInterface && InputHostname)
	{
		MenuInterface->Host(InputHostname->GetText().ToString());
	}
}

void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Join Menu"));
	if (!ensure(MenuWidgetSwitcher)) { return; }
	if (!ensure(JoinMenu)) { return; }
	MenuWidgetSwitcher->SetActiveWidget(JoinMenu);

	if (ServerScrollBox)
	{
		ServerScrollBox->ClearChildren();

		InfoMessage->SetText(FText::FromString(SEARCH_WAITING_TEXT));
	}

	if (MenuInterface)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::ConfirmJoin()
{
	JoinSelectedServer();
}

void UMainMenu::BackToMainMenu()
{
	if (!ensure(MenuWidgetSwitcher)) { return; }
	if (!ensure(MainMenu)) { return; }
	MenuWidgetSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	APlayerController* PC = World->GetFirstPlayerController();
	if (!ensure(PC)) { return; }

	PC->ConsoleCommand("Quit");
}

void UMainMenu::AddServerRow(const FServerData& Data, uint32 Index)
{
	if (!ensure(ServerRowClass)) { return; }
	UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
	if (!ensure(ServerRow)) { return; }

	ServerRow->ServerNameTextBlock->SetText(FText::FromString(Data.Name));
	ServerRow->HostUser->SetText(FText::FromString(Data.HostUsername));
	FString FractionText = FString::Printf(TEXT("%d/%d"), Data.CurrentPlayers, Data.MaxPlayers);
	ServerRow->ConnectionFraction->SetText(FText::FromString(FractionText));
	ServerRow->Setup(this, Index);

	if (!ensure(ServerScrollBox)) { return; }
	ServerScrollBox->AddChild(ServerRow);
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerScrollBox->GetChildrenCount(); ++i)
	{
		UServerRow* Row = Cast<UServerRow>(ServerScrollBox->GetChildAt(i));
		if (Row)
		{
			if (Row->GetIndex() == SelectedIndex)
			{
				Row->VisualizeRow(true);
			}
			else
			{
				Row->VisualizeRow(false);
			}
		}
	}
}
