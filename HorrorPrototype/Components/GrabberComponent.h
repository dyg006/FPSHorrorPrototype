// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "GrabberComponent.generated.h"

class AMainCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORPROTOTYPE_API UGrabberComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/// <summary>
	/// Grabs an item if possible.
	/// </summary>
	void Grab();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// The hit struct
	/// </summary>
	FHitResult hit;
	
	/// <summary>
	/// The trace collision parameters 
	/// </summary>
	FCollisionQueryParams traceParams;

	UPROPERTY()
	/// <summary>
	/// The physics handle
	/// </summary>
	UPhysicsHandleComponent* physicsHandle = nullptr;

	/// <summary>
	/// Gets the first physics body in reach.
	/// </summary>
	/// <returns></returns>
	FHitResult GetFirstPhysicsBodyInReach();
	
	/// <summary>
	/// Gets the player world position.
	/// </summary>
	/// <returns></returns>
	FVector GetPlayerWorldPos() const;
	
	/// <summary>
	/// Gets the position the player can reach.
	/// </summary>
	/// <returns></returns>
	FVector GetPlayerReach() const;

	/// <summary>
	/// The interactive components
	/// </summary>
	TArray<UInteractiveComponent*> interactiveComponents;

	/// <summary>
	/// The distance from the player in which an item can be grabbed
	/// </summary>
	float distance = 100.f;

	/// <summary>
	/// Checks if can grab an item.
	/// </summary>
	void CheckIfCanGrab();

	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;

	/// <summary>
	/// The time between the component checks for items in range
	/// </summary>
	const float TimeBetweenItemsCheck = 0.1f;

	/// <summary>
	/// The time lasts
	/// </summary>
	float timeLasts = 0.f;
};
