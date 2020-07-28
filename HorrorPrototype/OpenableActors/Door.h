// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Components/OpenableComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class AMainCharacter;

UCLASS()
class HORRORPROTOTYPE_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//The box collision of the actor
	class UBoxComponent* boxCollision;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	//The static mesh of the actor
	class UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere)
	//The openable component of the actor
	class UOpenableComponent* openableComponent;
	
	/// <summary>
	/// Notifies the actor end overlap.
	/// </summary>
	/// <param name="otherActor">The other actor.</param>
	void NotifyActorEndOverlap(AActor* otherActor) override;

	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;
};
