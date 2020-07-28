// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "MainCharacter.h"
//#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "MainCharacterController.generated.h"

class AHorrorPrototypeGameModeBase;

/**
 * 
 */
UCLASS()
class HORRORPROTOTYPE_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this controller's properties
	AMainCharacterController();
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, Category = Camera)
		float baseTurnRate = 90.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, Category = Camera)
		float baseLookUpRate = 90.f;

	/// <summary>
	/// Checks whether the player can look up
	/// </summary>
	bool canLookUp = true;

	/// <summary>
	/// The input mode used by the controller
	/// </summary>
	FInputModeGameAndUI inputModeGameAndUI;

	/// <summary>
	/// The input mode used by the controller
	/// </summary>
	FInputModeGameOnly inputModeGameOnly;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The perception source component
	/// </summary>
	UAIPerceptionStimuliSourceComponent* perceptionSourceComponent;
	
protected:
    void BeginPlay();

	/// <summary>
	/// Setups the input component.
	/// </summary>
	virtual void SetupInputComponent() override;

	/// <summary>
	/// The sprint is active
	/// </summary>
	bool sprintIsActive = false;

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

	/// <summary>
	/// Adds the controller pitch input.
	/// </summary>
	/// <param name="Val">The value.</param>
	void AddControllerPitchInput(float Val);

	/// <summary>
	/// Adds the controller yaw input.
	/// </summary>
	/// <param name="Val">The value.</param>
	void AddControllerYawInput(float Val);

	/* [L3 || Shift] Called in order to start running */
	void StartRunning();

	/* [L3 || Shift] Called in order to stop running */
	void StopRunning();

	/* [R2 || Left click] Called in order to execute an attack or push trap on world */
	void Shoot();

	/* [Square || R] Called in order to aim with a weapon */
	void Reload();

	/* [Start Button || Esc ] Pause the game */
	void Pause();

	/* [L2 || Right click ] Starts aiming*/
	void StartAiming();

	/* [L2 || Right click ] Stops aiming*/
	void StopAiming();

	/* [R3 || C ] Starts crouching*/
	void StartCrouching();

	/* [X || F ] Starts aiming*/
	void Interact();
	
private:
	/// <summary>
	/// The current pawn
	/// </summary>
	APawn* currentPawn;

	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;

	/// <summary>
	/// The main character movement
	/// </summary>
	UCharacterMovementComponent* mainCharacterMovement;

	/// <summary>
	/// The mouse sensitivity
	/// </summary>
	float mouseSensitivity = 0.7f;

	/// <summary>
	/// The game mode
	/// </summary>
	AHorrorPrototypeGameModeBase* gameMode;
};
