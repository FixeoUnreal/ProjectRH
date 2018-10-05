// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameStateBase.h"
#include <GameFramework/GameState.h>
#include "RHGameState.generated.h"

class ARHPlayerState;
class AWayGate;

/**
 * 
 */
UCLASS()
class PROJECTRH_API ARHGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	TArray<ARHPlayerState*> PositionList;

public:
	ARHGameState();

	void UpdateScoreBoard();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "RHGameState")
	TArray<ARHPlayerState*> GetPositionList() const;

	void AddNewPlayerToScoreBoard(ARHPlayerState* RHPlayerState);

	void InitializeScoreBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	bool bInitScoreBoard = false;

	TArray<AWayGate*> WayGateList;

};
