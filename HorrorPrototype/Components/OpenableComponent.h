// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODEvent.h"
#include "InteractiveComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBox.h"
#include "OpenableComponent.generated.h"

class AMainCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORPROTOTYPE_API UOpenableComponent : public UInteractiveComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenableComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/// <summary>
	/// Sets the openable owner actor to interacted or not.
	/// </summary>
	/// <param name="hasInteracted">if set to <c>true</c> [has interacted].</param>
	void SetInteracted(bool hasInteracted) { interacted = hasInteracted; }

	/// <summary>
	/// The player is on range
	/// </summary>
	bool playerIsOnRange = false;

	UPROPERTY(EditAnywhere)
	int keyId = -1; //check if the door need a key in order to open it, if -1 means that no key is need it

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The door sound
	/// </summary>
	UFMODEvent* doorSound;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The door locked sound
	/// </summary>
	UFMODEvent* doorLockedSound;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// The initial yaw
	/// </summary>
	float initialYaw;
	
	/// <summary>
	/// The current yaw
	/// </summary>
	float currentYaw;
	
	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The open angle
	/// </summary>
	float openAngle = 90.f;

	/// <summary>
	/// The door last opened
	/// </summary>
	float doorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The door close delay
	/// </summary>
	float doorCloseDelay = 0.6f;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The open velocity
	/// </summary>
	float openVelocity = 0.8f;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The close velocity
	/// </summary>
	float closeVelocity = 2.f;

	/// <summary>
	/// Checks if the actor has been opened at least one time
	/// </summary>
	bool actorOpened = false;

	/// <summary>
	/// Checks if the actor has been interacted
	/// </summary>
	bool interacted = false;
	
	/// <summary>
	/// Opens the door.
	/// </summary>
	/// <param name="DeltaTime">The delta time.</param>
	void OpenDoor(float DeltaTime);

	/// <summary>
	/// Closes the door.
	/// </summary>
	/// <param name="DeltaTime">The delta time.</param>
	void CloseDoor(float DeltaTime);

	/// <summary>
	/// Receives the interaction of the character.
	/// </summary>
	void ExecuteInteraction() override;

	/// <summary>
	/// Plays the door locked sound.
	/// </summary>
	void PlayDoorLockedSound();
	
	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;

	/// <summary>
	/// Checks if close door sound ready
	/// </summary>
	bool closeDoorSoundReady = false;

	/// <summary>
	/// Checks if the open door sound ready
	/// </summary>
	bool openDoorSoundReady = false;
	
	/// <summary>
	/// The angle factor that allows as to check if the door has already been opened
	/// </summary>
	const float angleFactor = 10.f;
};
