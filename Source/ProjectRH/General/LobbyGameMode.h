// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectRHGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRH_API ALobbyGameMode : public AProjectRHGameMode
{
	GENERATED_BODY()
	
public:
	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "LobbyGameMode")
	void TravelToSelectedMap(const FString MapPath);

private:
	uint32 PlayerCount = 0;

	void StartGame();
	
	
};
