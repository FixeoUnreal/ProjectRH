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

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	int32 GetPassedWayGateCount() const;

	int32 GetLapCount() const;

	float GetDistanceToNextWayGate() const;

	void SetPassedWayGateCount(int32 InPassedWayGateCount);

	void SetLapCount(int32 InLapCount);

	void SetDistanceToNextWayGate(float InDistanceToNextWayGate);

	AWayGate* GetNexWayGate() const;

	void SetNextWayGate(AWayGate* InWayGate);

	void UpdateNextWayGate();
	
protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "RHPlayerState")
	int32 PassedWayGateCount = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "RHPlayerState")
	int32 LapCount = 0;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "RHPlayerState")
	float DistanceToNextWayGate = 0.f;

	AWayGate* NextWayGate;

	int32 WayGateIndex = 0;
	
};
