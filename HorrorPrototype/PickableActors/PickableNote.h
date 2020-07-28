// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Components/PickableComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "PickableNote.generated.h"

UCLASS()
class HORRORPROTOTYPE_API APickableNote : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableNote();

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
	//The box collision of the actor
		class UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere)
	//The text render of the actor
	class UTextRenderComponent* textRender;

};
