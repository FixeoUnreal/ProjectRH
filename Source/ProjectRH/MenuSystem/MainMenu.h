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

/**
 * 
 */
UCLASS()
class PROJECTRH_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BtnHost;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BtnJoin;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnExit;

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void HostClicked();

	UFUNCTION()
	void JoinClicked();

	UFUNCTION()
	void QuitClicked();
};
