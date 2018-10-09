// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RHPlayerState.generated.h"

class AWayGate;

/**
 * 
 */
UCLASS()
class PROJECTRH_API ARHPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "RHPlayerState")
	TArray<AWayGate*> WayGateList;

	static const int32 FinalPosition_None = 100;

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	/*------------------Start Getter & Setter ------------------*/
	int32 GetPassedWayGateCount() const;

	int32 GetLapCount() const;

	float GetDistanceToNextWayGate() const;

	void SetPassedWayGateCount(int32 InPassedWayGateCount);

	void SetLapCount(int32 InLapCount);

	void SetDistanceToNextWayGate(float InDistanceToNextWayGate);

	UFUNCTION(BlueprintCallable, Category = "RHPlayerState")
	int32 GetFinalPosition() const;

	void SetFinalPosition(int32 InFinalPosition);

	AWayGate* GetNexWayGate() const;

	void SetNextWayGate(AWayGate* InWayGate);
	/*------------------End Getter & Setter ------------------*/

	void UpdateNextWayGate();

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "RHPlayerState")
	bool bIsReady = false;
	
protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "RHPlayerState")
	int32 PassedWayGateCount = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "RHPlayerState")
	int32 LapCount = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "RHPlayerState")
	float DistanceToNextWayGate = 0.f;

	// Position after crossing finish line
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "RHPlayerState")
	int32 FinalPosition = FinalPosition_None;

	AWayGate* NextWayGate;

	int32 WayGateIndex = 0;
	
};
