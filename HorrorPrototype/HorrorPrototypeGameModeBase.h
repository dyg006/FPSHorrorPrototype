// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacterController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "HorrorPrototypeGameModeBase.generated.h"

class AMainCharacter;

/**
 * 
 */
UCLASS()
class HORRORPROTOTYPE_API AHorrorPrototypeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHorrorPrototypeGameModeBase();
	
	/// <summary>
	/// Creates the pause menu.
	/// </summary>
	void CreatePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	/// <summary>
	/// Resumes the game.
	/// </summary>
	void ResumeGame();

	UPROPERTY()
	/// <summary>
	/// The main character controller
	/// </summary>
	AMainCharacterController* mainCharacterController;
	
	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The pause widget
	/// </summary>
	UUserWidget* PauseWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The lose widget
	/// </summary>
	UUserWidget* LoseWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The character widget
	/// </summary>
	UUserWidget* CharacterWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The UI pause widget
	/// </summary>
	TSubclassOf<UUserWidget> UIPauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The UI lose widget
	/// </summary>
	TSubclassOf<UUserWidget> UILoseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The UI character widget
	/// </summary>
	TSubclassOf<UUserWidget> UICharacterWidget;

	/// <summary>
	/// Gets if the is player dead.
	/// </summary>
	/// <returns></returns>
	bool GetIsPlayerDead() { return IsPlayerDead; }

	/// <summary>
	/// Sets the is player dead.
	/// </summary>
	/// <param name="isPlayerDead">if set to <c>true</c> [is player dead].</param>
	void SetIsPlayerDead(bool isPlayerDead);

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Creates the character widget.
	/// </summary>
	void CreateCharacterWidget();

	/// <summary>
	/// Shows the inventory.
	/// </summary>
	void ShowInventory();

	/// <summary>
	/// Hides the inventory.
	/// </summary>
	void HideInventory();

	/// <summary>
	/// Shows the no ammo message.
	/// </summary>
	void ShowNoAmmoMessage();

	/// <summary>
	/// Hides the no ammo message.
	/// </summary>
	void HideNoAmmoMessage();
	
	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The inventory visible
	/// </summary>
	bool inventoryVisible = false;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The no ammo message visible
	/// </summary>
	bool noAmmoMessageVisible = false;

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Removes the lose widget.
	/// </summary>
	void RemoveLoseWidget();

	UFUNCTION()
	/// <summary>
	/// Creates the lose widget.
	/// </summary>
	void CreateLoseWidget();

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The notes picked up
	/// </summary>
	int notesPickedUp = 0;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The number of notes to pick up
	/// </summary>
	int numberOfNotesToPickUp = 3;
	
protected:
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// The is player dead
	/// </summary>
	bool IsPlayerDead = false;
};
