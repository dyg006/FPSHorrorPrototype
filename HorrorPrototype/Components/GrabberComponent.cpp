// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"
#include "GameFramework/Actor.h"
#include "../MainCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	traceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	mainCharacter = Cast<AMainCharacter>(GetOwner());
	//FindPhysicsHandle();
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), hit.Location, FColor::Orange);
	timeLasts += DeltaTime;
	if(timeLasts >= TimeBetweenItemsCheck)
	{
		CheckIfCanGrab();
		timeLasts = 0.f;
	}
	
}

void UGrabberComponent::Grab() {

	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	//Only ray cast when the key is pressed
	AActor* hitActor = hitResult.GetActor();
	
	// If we hit something we check if is interactive first
	if (hitActor != nullptr) {
		UInteractiveComponent* interactiveComponent = hitActor->FindComponentByClass<UInteractiveComponent>();
		if (interactiveComponent != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *hitActor->GetName());

			//Destroy the actor on the world
			interactiveComponent->ExecuteInteraction();
		}
	}
}

void UGrabberComponent::CheckIfCanGrab()
{

	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	//Only ray cast when the key is pressed
	AActor* hitActor = hitResult.GetActor();

	// If we hit something we check if is interactive first
	if (hitActor != nullptr) {
		UInteractiveComponent* interactiveComponent = hitActor->FindComponentByClass<UInteractiveComponent>();
		if (interactiveComponent != nullptr) {

			if(mainCharacter != nullptr && !mainCharacter->itemInRange) {
				mainCharacter->itemInRange = true;
			}

		} else
		{
			if (mainCharacter != nullptr && mainCharacter->itemInRange) {
				mainCharacter->itemInRange = false;
			}
		}
	} else
	{
		if (mainCharacter != nullptr && mainCharacter->itemInRange) {
			mainCharacter->itemInRange = false;
		}
	}
}


FHitResult UGrabberComponent::GetFirstPhysicsBodyInReach() {

	// Ray-cast out to a certain distance (reach)
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetPlayerWorldPos(),
		GetPlayerReach(),
		FCollisionObjectQueryParams::AllDynamicObjects,//FCollisionObjectQueryParams::DefaultObjectQueryParam,
		traceParams
	);
	
	return hit;
}

FVector UGrabberComponent::GetPlayerWorldPos() const {
	FVector viewLocation;
	FRotator viewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT viewLocation,
		OUT viewRotation
	);
	return viewLocation;
}

FVector UGrabberComponent::GetPlayerReach() const {
	FVector viewLocation;
	FRotator viewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT viewLocation,
		OUT viewRotation
	);

	return viewLocation + viewRotation.Vector() * distance;
}
