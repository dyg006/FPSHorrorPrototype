// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Components/PickableComponent.h"
#include "Components/CapsuleComponent.h"
#include "AmmoPickup.generated.h"

UCLASS()
class HORRORPROTOTYPE_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();

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
	//The pickable component of the actor
	class UPickableComponent* pickableComponent;

	UPROPERTY(EditAnywhere)
	//The capsule collision component
	class UCapsuleComponent* capsuleCollision;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The ammo
	/// </summary>
	int ammo;
};
