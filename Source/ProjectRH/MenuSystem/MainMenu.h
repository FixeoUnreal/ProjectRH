// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class IMenuInterface;
class ULevel;
class UWorld;
class UWidgetSwitcher;
class UEditableTextBox;
class UInputComponent;
class UWidget;
class UUserWidget;
class FObjectInitializer;
class FOnlineSessionSearchResult;
//struct FServerData;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

		FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PROJECTRH_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void SetServerList(TArray<FServerData> ServerList);

	void SelectIndex(uint32 Index);

	void JoinSelectedServer();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BtnHost;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnJoin;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnJoinWithAddr;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnHostWithName;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnBackFromJoin;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnBackFromHost;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnQuit;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InfoMessage;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* InputHostname;

	TOptional<uint32> SelectedIndex;

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void ConfirmHost();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void ConfirmJoin();

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void QuitGame();

private:
	TSubclassOf<UUserWidget> ServerRowClass;

private:
	void AddServerRow(const FServerData& DisplayStr, uint32 Index);

	void UpdateChildren();
};
