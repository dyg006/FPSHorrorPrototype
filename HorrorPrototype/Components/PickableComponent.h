// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveComponent.h"
#include "PickableComponent.generated.h"

class AMainCharacter;

UENUM()
enum TypeOfItem
{
	Weapon UMETA(DisplayName = "Weapon"),
	Key    UMETA(DisplayName = "Key"),
	Ammo    UMETA(DisplayName = "Ammo"),
	Health    UMETA(DisplayName = "Health"),
	Note    UMETA(DisplayName = "Note")
};

/**
 * 
 */
UCLASS()
class HORRORPROTOTYPE_API UPickableComponent : public UInteractiveComponent
{
	GENERATED_BODY()

public:
	// Sets UPickableComponent values for this component's properties
	UPickableComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	/// <summary>
	/// The type of item
	/// </summary>
	TEnumAsByte<TypeOfItem> typeOfItem;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	/// <summary>
	/// Receives the interaction of the character.
	/// </summary>
	void ExecuteInteraction() override;

	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;
};
