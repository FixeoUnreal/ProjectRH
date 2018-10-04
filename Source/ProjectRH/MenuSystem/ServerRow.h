// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Geometry.h>
#include <Events.h>
#include "ServerRow.generated.h"

class UTextBlock;
class UButton;
class UMainMenu;
class FReply;

/**
 *
 */
UCLASS()
class PROJECTRH_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostUser;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionFraction;

public:
	/**
	/* Description:
	/* Returns:   void
	/* Parameter: Parent
	/* Parameter: Index - must be less than 0 to display only messages
	**/
	void Setup(UMainMenu* Parent, uint32 Index);

	UFUNCTION(BlueprintImplementableEvent, Category = "ServerRow")
	void VisualizeRow(bool bInSelected);

	uint32 GetIndex() const;

protected:
	uint32 RowIndex = -1;

	bool bSelected = false;

	UMainMenu* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* BtnChooseServer;

protected:
	UFUNCTION()
	void ChooseServerClicked();

private:
	float LastTimeClicked = -1;

};

