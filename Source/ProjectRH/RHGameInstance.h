// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "RHGameInstance.generated.h"

class FObjectInitializer;
class UMenuWidget;

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
	virtual void Host() override;

	UFUNCTION(Exec)
	virtual void Join() override;

	UFUNCTION(Exec)
	void OpenMainMenu() override;

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void LoadMenuWidget(TSubclassOf<UMenuWidget> MenuClass);

	UFUNCTION(BlueprintCallable, Category = "PPGameInstance")
	void ExitMenu();

private:
	UMenuWidget* CurrentMenu;
	
};
