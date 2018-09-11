// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MenuInGame.generated.h"

class UButton;
class IMenuInterface;

/**
 * 
 */
UCLASS()
class PROJECTRH_API UMenuInGame : public UMenuWidget
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BtnResume;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnQuit;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnMainMenu;

protected:
	virtual bool Initialize() override;
	
	UFUNCTION()
	void ResumeClicked();

	UFUNCTION()
	void QuitClicked();

	UFUNCTION()
	void MainMenuClicked();
};
