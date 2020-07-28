// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODEvent.h"
#include "../Components/PickableComponent.h"
#include "../Projectiles/Weapon_Projectile.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "PickableWeapon.generated.h"

class AHorrorPrototypeGameModeBase;
class AMainCharacter;

UCLASS()
class HORRORPROTOTYPE_API APickableWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Receives the order from the player to shoot.
	/// </summary>
	/// <param name="projectileSpawnRotation">The projectile spawn rotation.</param>
	/// <returns></returns>
	bool Action(FRotator projectileSpawnRotation);

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Plays the reload animation.
	/// </summary>
	void PlayReloadAnimation();

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Reloads this weapon.
	/// </summary>
	void Reload();
	
	UPROPERTY(EditAnywhere)
	//The skeletal mesh of the actor
	class USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(EditAnywhere)
	//The pickable component of this actor
	class UPickableComponent* pickableComponent;

	UPROPERTY(EditAnywhere)
	//The box collision of the actor
	class UBoxComponent* boxCollision;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AWeapon_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The total ammo of the weapon
	/// </summary>
	int totalAmmo = 20;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The ammo by charger
	/// </summary>
	int ammoByCharger = 12;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The reload sound
	/// </summary>
	UFMODEvent* reloadSound;

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Gets the current charger ammo.
	/// </summary>
	/// <returns></returns>
	const int GetCurrentChargerAmmo() const { return currentChargerAmmo; }

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Gets the current total ammo.
	/// </summary>
	/// <returns></returns>
	const int GetCurrentTotalAmmo() const { return currentTotalAmmo; }

	/// <summary>
	/// Increases the total ammo.
	/// </summary>
	/// <param name="extraAmmo">The extra ammo.</param>
	void IncreaseTotalAmmo(int extraAmmo) { currentTotalAmmo += extraAmmo; }
	
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	//The reload animation montage
	class UAnimMontage* reloadAnimMontage;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// Determines whether this instance can reload.
	/// </summary>
	/// <returns>
	///   <c>true</c> if this instance can reload; otherwise, <c>false</c>.
	/// </returns>
	bool CanReload();
	
	/// <summary>
	/// The current total ammo
	/// </summary>
	int currentTotalAmmo;

	/// <summary>
	/// The current charger ammo
	/// </summary>
	int currentChargerAmmo;

	/// <summary>
	/// The game mode
	/// </summary>
	AHorrorPrototypeGameModeBase* gameMode;

	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;
};
