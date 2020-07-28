// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableWeapon.h"
#include "../HorrorPrototypeGameModeBase.h"
#include "../MainCharacter.h"
#include "FMODBlueprintStatics.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickableWeapon::APickableWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = skeletalMesh;

	pickableComponent = CreateDefaultSubobject<UPickableComponent>(TEXT("Pickable"));
	//openableComponent->SetupAttachment(RootComponent);

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	boxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickableWeapon::BeginPlay()
{
	Super::BeginPlay();

	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	currentTotalAmmo = totalAmmo;
	if(currentTotalAmmo >= ammoByCharger)
	{
		currentChargerAmmo = ammoByCharger;
	} else
	{
		currentChargerAmmo = currentTotalAmmo;
	}
	gameMode = Cast<AHorrorPrototypeGameModeBase>(GetWorld()->GetAuthGameMode());
	
}

// Called every frame
void APickableWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APickableWeapon::Action(FRotator projectileSpawnRotation)
{
	if(currentChargerAmmo <= 0) {
		if (gameMode != nullptr && currentTotalAmmo <= 0) {
			gameMode->ShowNoAmmoMessage();
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Intentando disparar con municion actual en el cargador: %d"), currentChargerAmmo));
		PlayReloadAnimation();
		return false;
	}
	
	//// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetActorLocation();

	////Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//// spawn the projectile at the muzzle
	GetWorld()->SpawnActor<AWeapon_Projectile>(ProjectileClass, SpawnLocation, projectileSpawnRotation, ActorSpawnParams);

	--currentChargerAmmo;
	--currentTotalAmmo;
	if(currentChargerAmmo <= 0)
	{
		PlayReloadAnimation();
	}
	return true;
}

void APickableWeapon::PlayReloadAnimation()
{
	if (CanReload()) {
		if (mainCharacter != nullptr && mainCharacter->mainCharacterAnimationInstance != nullptr) {
			if (!mainCharacter->mainCharacterAnimationInstance->Montage_IsPlaying(reloadAnimMontage)) {
				mainCharacter->mainCharacterAnimationInstance->Montage_Play(reloadAnimMontage);
			}
		}
	}

}

bool APickableWeapon::CanReload()
{
	if (currentTotalAmmo <= 0 || currentChargerAmmo == currentTotalAmmo) { //Not enough ammo for reloading
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("No tienes suficiente municion para regargar, cantidad total: %d"), currentTotalAmmo));
		return false;
	}
	else if (currentChargerAmmo == ammoByCharger) //We have already the max ammo of the charger
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Ya tienes el maximo de municion por cargador: %d"), currentChargerAmmo));
		return false;
	} else
	{
		return true;
	}
}

void APickableWeapon::Reload()
{	
	if(currentTotalAmmo >= ammoByCharger)
	{
		currentChargerAmmo = ammoByCharger;
	} else
	{
		currentChargerAmmo = currentTotalAmmo;
	}

}