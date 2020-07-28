// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORPROTOTYPE_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//// Sets default values for this component's properties
	UInteractiveComponent();

	//// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// Receives the interaction of the character.
	/// </summary>
	virtual void ExecuteInteraction() {};
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
