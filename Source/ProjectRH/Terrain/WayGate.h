// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WayGate.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECTRH_API AWayGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWayGate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditInstanceOnly, Category = "WayGate")
	int32 Index = -1;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMeshCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	
};
