// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "GameFramework/Actor.h"
#include "../Components/PickableComponent.h"
#include "Components/CapsuleComponent.h"
#include "HealthPickup.generated.h"

UCLASS()
class HORRORPROTOTYPE_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	//The static mesh of the actor
	class UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere)
	//The second static mesh of the actor
	class UStaticMeshComponent* staticMesh2;

	UPROPERTY(EditAnywhere)
	//The pickable component of the actor
	class UPickableComponent* pickableComponent;

	UPROPERTY(EditAnywhere)
	//The capsule collision of the actor
	class UCapsuleComponent* capsuleCollision;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The health that restores when picked
	/// </summary>
	float health;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The health pickup sound
	/// </summary>
	UFMODEvent* healthPickupSound;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The health pickup negation sound
	/// </summary>
	UFMODEvent* healthPickupNegationSound;
};
