// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "FMODBlueprintStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "HorrorPrototypeGameModeBase.h"
#include "Perception/AISense_Hearing.h"
#include "PickableActors/AmmoPickup.h"
#include "PickableActors/KeyActor.h"
#include "PickableActors/PickableWeapon.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create a CameraComponent	
	firstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	firstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	firstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	firstPersonCameraComponent->bUsePawnControlRotation = true;
	
	grabberComponent = CreateDefaultSubobject<UGrabberComponent>(TEXT("GrabberComponent"));
	grabberComponent->SetupAttachment(RootComponent);
	
	mainSoundtrack = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("MainSoundtrackComponent"));
	mainSoundtrack->SetupAttachment(RootComponent);
	
	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthPlayerComponent"));
	healthComponent->Init(100);
	
	pawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));
	
	heartbeatSound = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("HeartbeatSoundComponent"));
	heartbeatSound->SetupAttachment(RootComponent);
	
	mainCharacterMeshComponent = GetMesh();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateHeartbeatSound();
	
	gameMode = Cast<AHorrorPrototypeGameModeBase>(GetWorld()->GetAuthGameMode());

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("El player tiene: %lf de vida"), heartbeatSound->GetParameter("Health")));
	
	if (aiming) {
		if (firstPersonCameraComponent->FieldOfView != zoomView) {
			firstPersonCameraComponent->FieldOfView = FMath::FInterpTo(firstPersonCameraComponent->FieldOfView, zoomView, DeltaTime, interpAimSpeed);
		}
	} else {
		if (firstPersonCameraComponent->FieldOfView != normalView) {
			firstPersonCameraComponent->FieldOfView = FMath::FInterpTo(firstPersonCameraComponent->FieldOfView, normalView, DeltaTime, interpAimSpeed);
		}
	}
}

void AMainCharacter::Crawl() {
	if(isCrouching) {
		GetCharacterMovement()->MaxWalkSpeed = walkingSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(maxHeight);
		//SetActorScale3D(FVector(1.0, 1.0, 1.0));
		isCrouching = false;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = crouchedSpeed;
		GetCapsuleComponent()->SetCapsuleHalfHeight(crouchedHeight);
		//SetActorScale3D(FVector(1.0, 1.0, 0.2));
		isCrouching = true;
	}
	
}

void AMainCharacter::Interact()
{
	if(grabberComponent != nullptr)
	{
		grabberComponent->Grab();
	}
}

void AMainCharacter::AddToInventory(AActor* newItem, int typeOfItem)
{
	
	switch(typeOfItem)
	{
		case TypeOfItem::Weapon:
			TakeWeapon(newItem);
			break;
		case TypeOfItem::Key:
			PlayPickItemSound();
			gameMode->ShowInventory();
			newItem->SetActorHiddenInGame(true);
		break;
		case TypeOfItem::Ammo:
			PlayPickItemSound();
			TakeAmmo(newItem);
			break;
		case TypeOfItem::Note:
			PlayPickItemSound();
			newItem->SetActorHiddenInGame(true);
			if(gameMode != nullptr) {
				++gameMode->notesPickedUp;
			}
			break;
		default:
			break;
	}
		
	inventory.Add(newItem);
}

bool AMainCharacter::IsKeyOnInventory(int keyId) const
{
	for(AActor *item : inventory)
	{
		AKeyActor* keyActor = Cast<AKeyActor>(item);
		if(keyActor != nullptr && keyActor->keyId == keyId)
		{
			return true;
		}
	}
	return false;
}

void AMainCharacter::RemoveKeyFromInventory(const int keyId)
{
	for (AActor* item : inventory)
	{
		AKeyActor* key = Cast<AKeyActor>(item);
		if (key != nullptr) {
			if (key->keyId != -1 && key->keyId == keyId)
			{
				gameMode->HideInventory();
				inventory.Remove(item);
				break;
			}
		}
	}
}

float AMainCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// DamageType example and properties
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	DamageTypeCDO->DamageImpulse;
	DamageTypeCDO->bCausedByWorld;

	if (ActualDamage != 0.f)
	{
		this->healthStatusEvent.Broadcast(this->healthComponent->health);
		healthComponent->Damage(ActualDamage);

		if(playerHurt != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(playerHurt);
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("El player tiene: %lf de vida"), healthComponent->health));
		// if dead destroy
		if (!healthComponent->alive) {

			//Activate the death animation of the player (it is used as a delay too)		
			if (gameMode != nullptr)
			{
				if (!gameMode->GetIsPlayerDead()) {
			//		//timeWaitingForRespawn = 0.f;
					gameMode->SetIsPlayerDead(true);
				}
			}

			ActualDamage = 0.f;
			if (heartbeatSound != nullptr) {
				// set the parameter that control the beat sound to 1 again so the sound is not activated again when the player is dead
				heartbeatSound->SetParameter("Health", 1);
			}
		}
		else {
			UpdateHeartbeatSound();
		}
	}

	return ActualDamage;
	
}

void AMainCharacter::Shoot()
{
	if(currentWeapon != nullptr && !isReloading) {
		UE_LOG(LogTemp, Warning, TEXT("Weapon equipped trying to shoot"));
		const FRotator SpawnRotation = GetControlRotation();
		
		bool canShoot = currentWeapon->Action(SpawnRotation);
		if(canShoot)
		{
			MakeCharacterNoise(1.f);
			PlayShootAnimation();
		}
	}
}

void AMainCharacter::ReloadAction()
{
	if(currentWeapon != nullptr){
		currentWeapon->PlayReloadAnimation();
	}
}

void AMainCharacter::PlayPickItemSound()
{
	if(itemPicked != nullptr) {
		UFMODBlueprintStatics::PlayEventAttached(itemPicked, GetMesh(), "", GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
	}
}

void AMainCharacter::PlayPickWeaponSound()
{
	if (weaponPicked != nullptr) {
		UFMODBlueprintStatics::PlayEventAttached(weaponPicked, GetMesh(), "", GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
	}
}

void AMainCharacter::StartAiming()
{
	if(firstPersonCameraComponent != nullptr && currentWeapon != nullptr) {
		GetCharacterMovement()->MaxWalkSpeed = crouchedSpeed;
		aiming = true;
		//firstPersonCameraComponent->FieldOfView = zoomView;
	}
	
}

void AMainCharacter::StopAiming()
{
	if (firstPersonCameraComponent != nullptr) {
		GetCharacterMovement()->MaxWalkSpeed = walkingSpeed;
		aiming = false;
		//firstPersonCameraComponent->FieldOfView = normalView;
	}
}

void AMainCharacter::PlayShootAnimation()
{
	if (mainCharacterMeshComponent != nullptr) {
		mainCharacterAnimationInstance = mainCharacterMeshComponent->GetAnimInstance();

		if (!mainCharacterAnimationInstance->Montage_IsPlaying(shootAnimMontage)) {
			mainCharacterAnimationInstance->Montage_Play(shootAnimMontage);
		}
	}
}

void AMainCharacter::UpdateHeartbeatSound() {
	if (heartbeatSound != nullptr) {
	//	// set the parameter that control the beat sound normalizing the health value
		heartbeatSound->SetParameter("Health", healthComponent->health / healthComponent->defaultHealth);
	}
}

void AMainCharacter::ResetCharacterParams() {
	healthComponent->health = healthComponent->defaultHealth;

	UpdateHeartbeatSound();
}

void AMainCharacter::MakeCharacterNoise(float volume)
{
	pawnNoiseEmitterComp->MakeNoise(this, volume, GetActorLocation());
}

void AMainCharacter::TakeAmmo(AActor* ammoPickupActor)
{
	AAmmoPickup* ammoPickup = Cast<AAmmoPickup>(ammoPickupActor);
	if (ammoPickup != nullptr) {
		ammoPickup->SetActorHiddenInGame(true);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("AMMO RECOGIDAA!")));
		if (currentWeapon != nullptr) { //if we have a weapon equipped
			currentWeapon->IncreaseTotalAmmo(ammoPickup->ammo);
		}
		else {
			ammoOnInventory += ammoPickup->ammo;
		}
	}
}

void AMainCharacter::TakeWeapon(AActor* weaponPickupActor)
{
	APickableWeapon* weapon = Cast<APickableWeapon>(weaponPickupActor);
	UE_LOG(LogTemp, Warning, TEXT("Trying to equip the weapon"));
	PlayPickWeaponSound();
	currentWeapon = weapon;
	weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "GripPoint"); //FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
	GetMesh()->SetHiddenInGame(false);
	weapon->SetActorHiddenInGame(false);
	weapon->IncreaseTotalAmmo(ammoOnInventory);
}