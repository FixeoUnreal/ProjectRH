// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/RHAttributeSet.h"
#include "ProjectRHCharacter.generated.h"

class ASHPowerUp;
class UBoxComponent;
class USpeedComponent;
class URHAbilitySystemComponent;
class UAbilitySystemComponent;

UENUM(BlueprintType) enum class AbilityInput : uint8
{
	NormalSkill UMETA(DisplayName = "Normal Skill"),
	UltimateSkill UMETA(DisplayName = "Ultimate Skill"),
	PassiveSkill UMETA(DisplayName = "Passive Skill"),
};

UCLASS(config=Game)
class AProjectRHCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AProjectRHCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Automatically running forward
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "RHCharacter")
	bool bInRunMode = false;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void ActivatePowerUp();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerActivatePowerUp();

	UFUNCTION(BlueprintCallable, Category = "RHCharacter")
	void ForceMoveForward();

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Powerup")
	ASHPowerUp* PowerUpInstance;

	// In zone overlapping actors can be "attacked" by this character
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* AttackZone;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "RHCharacter")
	FVector DesiredForwardVector;

	UPROPERTY(BlueprintReadOnly, Category = "RHCharacter")
	float MoveForwardValue = 0;

	UPROPERTY(BlueprintReadOnly, Category = "RHCharacter")
	float MoveRightValue = 0;

	UPROPERTY(BlueprintReadOnly, Category = "RHCharacter")
	float BaseWalkSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpeedComponent* SpeedComp;

	/** Our ability system */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true")) 
	URHAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> NormalAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> UltimateAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> PassiveAbility;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	URHAttributeSet* AttributeSet;

	/** If true we have initialized our abilities */
	UPROPERTY()
	bool bAbilitiesInitialized = false;

protected:
	void ResetMoveForwardValue();

	void ResetMoveRightValue();

	// Update the distance to next WayGate in PlayerState
	UFUNCTION()
	void UpdateDistanceToNextWayGate();

	// Initializing abilities
	void AddStartupAbilities();

	/**
	 * Called when movement speed is changed
	 *
	 * @param DeltaValue Change in move speed
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Called from RPGAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AProjectRHCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Friended to allow access to handle functions above
	friend URHAttributeSet;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetPowerUp(ASHPowerUp* PowerUpToSet);

	UFUNCTION(BlueprintPure, Category = "RHCharacter")
	UBoxComponent* GetAttackZone() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	USpeedComponent* GetSpeedComp() const;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

private:
	// Prevent character from activating the same Powerup again 
	bool bPowerUpActivated = false;

	FTimerHandle TimerHandle_UpdateDistanceToNextWaveGate;
};

