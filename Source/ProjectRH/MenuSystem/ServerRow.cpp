// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "ServerRow.h"
#include "MainMenu.h"
#include <Button.h>
#include <Reply.h>


void UServerRow::Setup(UMainMenu* Parent, uint32 Index)
{
	// For displaying messages if no result
	if (Index < 0)
	{
		BtnChooseServer->SetIsEnabled(false);
		return;
	}

	RowIndex = Index;
	MainMenu = Parent;
	BtnChooseServer->OnClicked.AddDynamic(this, &UServerRow::ChooseServerClicked);
}

uint32 UServerRow::GetIndex() const
{
	return RowIndex;
}

void UServerRow::ChooseServerClicked()
{
	if (!ensure(MainMenu)) { return; }
	MainMenu->SelectIndex(RowIndex);

	// Handle double click
	if (GetWorld()->GetTimeSeconds() - LastTimeClicked < 0.25)
	{
		MainMenu->JoinSelectedServer();
		return;
	}

	bSelected = true;

	LastTimeClicked = GetWorld()->GetTimeSeconds();
}



