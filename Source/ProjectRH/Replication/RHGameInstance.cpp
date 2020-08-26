// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHGameInstance.h"
#include <Engine/Engine.h>
#include <OnlineSessionSettings.h>
#include <GameFramework/PlayerController.h>
#include <ConstructorHelpers.h>
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuInGame.h"
#include "MenuSystem/MenuWidget.h"
#include <GameMapsSettings.h>


const static FName SESSION_NAME = NAME_GameSession;
const static FName SESSION_SETTINGS_KEY_SERVER_NAME = TEXT("RHServerName");

URHGameInstance::URHGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenBPClass(TEXT("/Game/UI/WBP_LoadingScreen"));
	if (!ensure(LoadingScreenBPClass.Class)) { return; }
	LoadingScreenClass = LoadingScreenBPClass.Class;
}

void URHGameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URHGameInstance::OnCreateSessionCompleted);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &URHGameInstance::OnDestroySessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URHGameInstance::OnFindSessionsCompleted);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URHGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found!"));
	}

}

void URHGameInstance::Host(const FString& Name)
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			bIsHosting = true;
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			DesiredServerName = Name;
			CreateSession();
		}
	}
}

void URHGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings = ConfigureSessionSettings();

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}


FOnlineSessionSettings URHGameInstance::ConfigureSessionSettings()
{
	FOnlineSessionSettings SessionSettings;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SESSION_SETTINGS_KEY_SERVER_NAME, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	return SessionSettings;
}

void URHGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) { return; }
	TearDownCurrentMenu();

	DestroyExistingSession();

	ShowLoadingScreen();

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void URHGameInstance::ShowLoadingScreen()
{
	if (LoadingScreenClass)
	{
		UUserWidget* LoadingScreen = CreateWidget<UUserWidget>(this, LoadingScreenClass);
		LoadingScreen->AddToViewport();
	}
}

void URHGameInstance::TearDownCurrentMenu()
{
	if (CurrentMenu) {
		CurrentMenu->TearDown();
	}
}

void URHGameInstance::DestroyExistingSession()
{
	// Destroy session if already existed
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
			UE_LOG(LogTemp, Warning, TEXT("DEstroy after join called"));
		}
	}
}

void URHGameInstance::OpenMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }
	PC->ClientTravel("/Game/Maps/MainMenu_Map", ETravelType::TRAVEL_Absolute);
}

void URHGameInstance::RefreshServerList()
{
	if (!SessionInterface.IsValid()) { return; }
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 10000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Start find sessions"));
	}
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

void URHGameInstance::OpenMainMenuWithBP()
{
	OpenMainMenu();
}

FString URHGameInstance::GetServerName() const
{
	FString ServerName;
	SessionInterface->GetSessionSettings(SESSION_NAME)->Get(SESSION_SETTINGS_KEY_SERVER_NAME, ServerName);
	return ServerName;
}

void URHGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void URHGameInstance::OnCreateSessionCompleted(FName SessionName, bool Success)
{
	if (Success)
	{
		StartHostSession(SessionName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not create session"));
	}
}

void URHGameInstance::StartHostSession(FName SessionName)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, TEXT("Hosting " + SessionName.ToString()));

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	World->ServerTravel("/Game/Maps/Lobby_Map?listen");

}

void URHGameInstance::OnDestroySessionCompleted(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySession success"));

		// Client won't create a new session after leaving sessions
		if (bIsHosting) {
			bIsHosting = false;
			return;
		}

		CreateSession();
	}
}

void URHGameInstance::OnFindSessionsCompleted(bool bWasSuccessful)
{
	// Pass data of all found "servers" to main menu
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		TArray<FServerData> ServerDataItems = GetServers();

		if (!ensure(CurrentMenu)) { return; }
		UMainMenu* MainMenu = Cast<UMainMenu>(CurrentMenu);
		if (!ensure(MainMenu)) { return; }
		MainMenu->SetServerList(ServerDataItems);
	}
}

TArray<FServerData> URHGameInstance::GetServers()
{
	TArray<FServerData> ServerDataItems;
	for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
	{
		FServerData Data;
		Data.HostUsername = Result.Session.OwningUserName;
		Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - Result.Session.NumOpenPublicConnections;
		FString ServerName;
		if (Result.Session.SessionSettings.Get(SESSION_SETTINGS_KEY_SERVER_NAME, ServerName))
		{
			Data.Name = ServerName;
		}
		else
		{
			Data.Name = "Could not find name";
			UE_LOG(LogTemp, Warning, TEXT("Could not get expected session settings value"));
		}
		ServerDataItems.Add(Data);
	}

	return ServerDataItems;
}

void URHGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success
		&& SessionInterface.IsValid())
	{
		FString ConnectionInfo;
		bool Succecss = SessionInterface->GetResolvedConnectString(SessionName, ConnectionInfo);
		if (!Succecss || ConnectionInfo.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get connect string!"));
			return;
		}

		JoinMatch(ConnectionInfo);
	}
}

void URHGameInstance::JoinMatch(FString ConnectionInfo)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	Engine->AddOnScreenDebugMessage(0, 2.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *ConnectionInfo));

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC)) { return; }

	PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
}

