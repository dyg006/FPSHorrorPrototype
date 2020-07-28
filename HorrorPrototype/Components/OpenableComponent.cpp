// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenableComponent.h"

#include "FMODBlueprintStatics.h"
#include "../MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UOpenableComponent::UOpenableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenableComponent::BeginPlay()
{
	Super::BeginPlay();

	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	openAngle += initialYaw;

	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}


// Called every frame
void UOpenableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if the door has received a interaction by player
	if (interacted) {
		//if key is not needed in order to open the actor
		if (keyId == -1) { 
			OpenDoor(DeltaTime);
			doorLastOpened = GetWorld()->GetTimeSeconds();
		} else {
			if(mainCharacter != nullptr && mainCharacter->IsKeyOnInventory(keyId)) {
				OpenDoor(DeltaTime);
				mainCharacter->RemoveKeyFromInventory(keyId);				
				keyId = -1; //The door do not need a key anymore
				doorLastOpened = GetWorld()->GetTimeSeconds();	
			} else { //if the player is trying to open a door but the key is not on inventory
				PlayDoorLockedSound();
				interacted = false;
			}
		}
	} else {
		if (GetWorld()->GetTimeSeconds() - doorLastOpened > doorCloseDelay && !playerIsOnRange) {
			CloseDoor(DeltaTime);
		}
	}

	//Check if door has been totally opened
	if(currentYaw + angleFactor >= openAngle) {
		actorOpened = true;
	}
}

void UOpenableComponent::OpenDoor(float DeltaTime) {
	//currentYaw = FMath::FInterpTo(currentYaw, openAngle, DeltaTime, interpSpeed);
	currentYaw = FMath::Lerp(currentYaw, openAngle, DeltaTime * openVelocity);
	FRotator doorRotation = GetOwner()->GetActorRotation();
	doorRotation.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotation);

	closeDoorSoundReady = false;
	if (doorSound != nullptr) {
		if (!openDoorSoundReady) {
			UFMODBlueprintStatics::PlayEventAttached(doorSound, GetOwner()->GetRootComponent(), "", GetOwner()->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
			//openAudio->Play();
			openDoorSoundReady = true;
		}
	}
}

void UOpenableComponent::CloseDoor(float DeltaTime) {
	//currentYaw = FMath::FInterpTo(currentYaw, openAngle, DeltaTime, interpSpeed);
	currentYaw = FMath::Lerp(currentYaw, initialYaw, DeltaTime * closeVelocity);
	FRotator doorRotation = GetOwner()->GetActorRotation();
	doorRotation.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotation);

	openDoorSoundReady = false;
	if (doorSound != nullptr) {
		if (!closeDoorSoundReady && actorOpened) {
			UFMODBlueprintStatics::PlayEventAttached(doorSound, GetOwner()->GetRootComponent(), "", GetOwner()->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
			//openAudio->Play();
			closeDoorSoundReady = true;
		}
	}
}

void UOpenableComponent::ExecuteInteraction()
{
	interacted = true;
}

void UOpenableComponent::PlayDoorLockedSound()
{
	if (doorLockedSound != nullptr) {
		UFMODBlueprintStatics::PlayEventAttached(doorLockedSound, GetOwner()->GetRootComponent(), "", GetOwner()->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
	}
}