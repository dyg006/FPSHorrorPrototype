// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "Components/GrabberComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PickableActors/PickableWeapon.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "MainCharacter.generated.h"

class AHorrorPrototypeGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthStatusDelegate, float, currentHealth);

UCLASS()
class HORRORPROTOTYPE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
		FHealthStatusDelegate healthStatusEvent;

	//Used to allow the vibration of the controller
	UPROPERTY(EditAnywhere, Category = "Character")
		class UForceFeedbackEffect* playerHurt;
	
	/* Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		UHealthComponent* healthComponent;
	
	/// <summary>
	/// Checkes whether the player can execute action
	/// </summary>
	bool canExecuteAction = true;

	UPROPERTY(EditAnywhere, Category = "Character")
	/// <summary>
	/// The runnning speed
	/// </summary>
	float runnningSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	/// <summary>
	/// The walking speed
	/// </summary>
	float walkingSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	/// <summary>
	/// The crouched speed
	/// </summary>
	float crouchedSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Sound")
	//The main soundtrack audio component
	class UFMODAudioComponent* mainSoundtrack;

	/// <summary>
	/// Tries to crawl if possible.
	/// </summary>
	void Crawl();

	/// <summary>
	/// Interacts with an object of the map.
	/// </summary>
	void Interact();

	/// <summary>
	/// Gets if is crouching or not.
	/// </summary>
	/// <returns></returns>
	bool GetIsCrouching() const { return isCrouching; }

	/// <summary>
	/// Adds to inventory.
	/// </summary>
	/// <param name="newItem">The new item.</param>
	/// <param name="typeOfItem">The type of item.</param>
	void AddToInventory(AActor* newItem, int typeOfItem);

	/// <summary>
	/// Determines whether [is key on inventory] [the specified key identifier].
	/// </summary>
	/// <param name="keyId">The key identifier.</param>
	/// <returns>
	///   <c>true</c> if [is key on inventory] [the specified key identifier]; otherwise, <c>false</c>.
	/// </returns>
	bool IsKeyOnInventory(int keyId) const;

	/// <summary>
	/// Takes the damage.
	/// </summary>
	/// <param name="Damage">The damage.</param>
	/// <param name="DamageEvent">The damage event.</param>
	/// <param name="EventInstigator">The event instigator.</param>
	/// <param name="DamageCauser">The damage causer.</param>
	/// <returns></returns>
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
	/// <summary>
	/// Removes the key from inventory.
	/// </summary>
	/// <param name="keyId">The key identifier.</param>
	void RemoveKeyFromInventory(const int keyId);

	/// <summary>
	/// Shoots if possible.
	/// </summary>
	void Shoot();

	/// <summary>
	/// The maximum height of the player
	/// </summary>
	float maxHeight = 88.f;

	/// <summary>
	/// The crouched height of the player
	/// </summary>
	float crouchedHeight = 10.f;

	/// <summary>
	/// Reloads the action.
	/// </summary>
	void ReloadAction();

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The current weapon equipped
	/// </summary>
	APickableWeapon* currentWeapon = nullptr;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The item picked sound
	/// </summary>
	UFMODEvent* itemPicked;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The weapon picked sound
	/// </summary>
	UFMODEvent* weaponPicked;

	/// <summary>
	/// Starts the aiming if possible.
	/// </summary>
	void StartAiming();

	/// <summary>
	/// Stops the aiming if possible.
	/// </summary>
	void StopAiming();

	/*A Pawn Noise Emitter component which is used in order to emit the sounds to nearby AIs*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPawnNoiseEmitterComponent* pawnNoiseEmitterComp;

	/// <summary>
	/// Updates the heartbeat sound.
	/// </summary>
	void UpdateHeartbeatSound();

	/// <summary>
	/// Resets the character parameters.
	/// </summary>
	void ResetCharacterParams();

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// Checks whether the item is in range or not
	/// </summary>
	bool itemInRange = false;

	/// <summary>
	/// The common main character animation
	/// </summary>
	UAnimInstance* mainCharacterAnimationInstance;

	UPROPERTY(BlueprintReadWrite)
	/// <summary>
	/// The is reloading
	/// </summary>
	bool isReloading = false;

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Makes a noise so that the AI can detect the player.
	/// </summary>
	/// <param name="volume">The volume.</param>
	void MakeCharacterNoise(float volume);

	/// <summary>
	/// The ammo on inventory
	/// </summary>
	int ammoOnInventory = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* firstPersonCameraComponent;

	/// <summary>
	/// Shoot anim montage of the common main character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* shootAnimMontage;

	/// <summary>
	/// Main Character skeletal mesh component
	/// </summary>
	USkeletalMeshComponent* mainCharacterMeshComponent = nullptr;

	/** Heartbeat sound component of the character */
	UPROPERTY(EditAnywhere, Category = Sound)
	class UFMODAudioComponent* heartbeatSound;
	
private:
	/// <summary>
	/// The game mode
	/// </summary>
	AHorrorPrototypeGameModeBase* gameMode;
	
	/// <summary>
	/// Checks if the player is currently crouching
	/// </summary>
	bool isCrouching = false;

	/// <summary>
	/// The grabber component
	/// </summary>
	UGrabberComponent* grabberComponent;

	/// <summary>
	/// The inventory
	/// </summary>
	TArray<AActor*> inventory;

	/// <summary>
	/// Plays the pick item sound.
	/// </summary>
	void PlayPickItemSound();

	/// <summary>
	/// Plays the pick weapon sound.
	/// </summary>
	void PlayPickWeaponSound();

	/// <summary>
	/// The zoom view
	/// </summary>
	const float zoomView = 70.f;

	/// <summary>
	/// The normal view
	/// </summary>
	const float normalView = 90.f;

	/// <summary>
	/// Checks whether the player is aiming or not
	/// </summary>
	bool aiming = false;

	/// <summary>
	/// The interpolate aim speed
	/// </summary>
	const float interpAimSpeed = 2.f;

	/// <summary>
	/// Plays the shoot animation.
	/// </summary>
	void PlayShootAnimation();

	/// <summary>
	/// Takes the ammo.
	/// </summary>
	/// <param name="ammoPickupActor">The ammo pickup actor.</param>
	void TakeAmmo(AActor* ammoPickupActor);
	
	/// <summary>
	/// Takes the weapon.
	/// </summary>
	/// <param name="weaponPickupActor">The weapon pickup actor.</param>
	void TakeWeapon(AActor* weaponPickupActor);
};
