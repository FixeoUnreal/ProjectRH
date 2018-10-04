// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include <OnlineSessionInterface.h>
#include "RHGameInstance.generated.h"

class FObjectInitializer;
class UMenuWidget;
class IOnlineSubsystem;
class FOnlineSessionSearch;
class UUserWidget;

/**
 * 
 */
UCLASS()
class PROJECTRH_API URHGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	URHGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(Exec)
	virtual void Host(const FString& Name) override;

	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	UFUNCTION(Exec)
	void OpenMainMenu() override;

	virtual void RefreshServerList() override;

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void LoadMenuWidget(TSubclassOf<UMenuWidget> MenuClass);

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void ExitMenu();

	void StartSession();

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Success);

	void StartHostSession(FName SessionName);

	void OnDestroySessionCompleted(FName SessionName, bool Success);

	void OnFindSessionsCompleted(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();


private:
	TSubclassOf<UUserWidget> MainMenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	TSubclassOf<UUserWidget> LoadingScreenClass;

	UMenuWidget* CurrentMenu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	bool bIsHosting = false;

	FString DesiredServerName;
	
};
