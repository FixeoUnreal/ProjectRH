// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RHObstacle.generated.h"

class UStaticMeshComponent;
class UParticleSystem;
class UBoxComponent;
class AProjectRHCharacter;
class UCharacterMovementComponent;
class USceneComponent;

UCLASS()
class PROJECTRH_API ARHObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARHObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastBeginOverlap();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastApplyCollisionEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "RHObstacle")
	void OnBeginOverlap(AProjectRHCharacter* OverlappingCharacter);

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void ApplyCollisionEffect(){}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* Scene;

	float OriginalWalkSpeed = 0.f;

	UCharacterMovementComponent* CharMovementComp;

	AProjectRHCharacter* OverlappingCharacter;

	UPROPERTY(EditAnywhere, Category = "RHObstacle")
	bool bInvisibleAfterOverlap =false;

	// Won't have any effect if the subclass doesn't implement the destroy mechanism using this variable
	UPROPERTY(EditAnywhere, Category = "RHObstacle")
	bool bDestroyAfterOverlap = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	
};
