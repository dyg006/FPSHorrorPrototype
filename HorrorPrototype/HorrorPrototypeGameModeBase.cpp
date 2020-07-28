// Copyright Epic Games, Inc. All Rights Reserved.


#include "HorrorPrototypeGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AHorrorPrototypeGameModeBase::AHorrorPrototypeGameModeBase()
{
	
}

void AHorrorPrototypeGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    mainCharacterController = Cast<AMainCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void AHorrorPrototypeGameModeBase::CreatePauseMenu() {
	
    if (UGameplayStatics::IsGamePaused(this)) {
     /*   if ((UIPauseWidget != nullptr || UIOptionsWidget != nullptr || UIGlosaryWidget != nullptr)
            && CurrentWidget) {
            UE_LOG(LogTemp, Warning, TEXT("Deactivating PAUSE"));
            necroCharacter->canExecuteAction = true;*/
        if (UIPauseWidget != nullptr) {
            ResumeGame();
        }
        /*}*/
    }
    else {
        if (UIPauseWidget != nullptr) {
            PauseWidget = CreateWidget<UUserWidget>(mainCharacterController, UIPauseWidget);
            PauseWidget->AddToViewport();
            //CurrentWidget = PauseWidget;

            mainCharacterController->inputModeGameAndUI.SetHideCursorDuringCapture(true);
            mainCharacterController->inputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            //mainCharacterController->inputModeGameAndUI.SetWidgetToFocus(PauseWidget->TakeWidget());

            mainCharacterController->SetInputMode(mainCharacterController->inputModeGameAndUI);

            //mainCharacter->canExecuteAction = false;

            PauseWidget->SetKeyboardFocus();
            mainCharacterController->bShowMouseCursor = true;

            UGameplayStatics::SetGamePaused(this, true);
            UE_LOG(LogTemp, Warning, TEXT("Activating pause widget"));

            /*   if (musicManager != nullptr) {
                   musicManager->TurnOffGeneralVolume(necroCharacter);
               }*/
        }
    }
}


void AHorrorPrototypeGameModeBase::ResumeGame()
{
    if (PauseWidget)
    {
	    mainCharacterController->inputModeGameOnly.SetConsumeCaptureMouseDown(false);
	    mainCharacterController->SetInputMode(mainCharacterController->inputModeGameOnly);

	    mainCharacterController->bShowMouseCursor = false;
	    PauseWidget->RemoveFromParent();
	    PauseWidget = nullptr;

	    UGameplayStatics::SetGamePaused(this, false);
	    UE_LOG(LogTemp, Warning, TEXT("Deactivating pause widget"));

      /*  if (musicManager != nullptr) {
            musicManager->TurnOnGeneralVolume();
        }*/

        // esto no sirve: deprecaca
        //necroCharacter->canExecuteAction = true;
    }
}

void AHorrorPrototypeGameModeBase::SetIsPlayerDead(bool isPlayerDead)
{
    IsPlayerDead = isPlayerDead;
	if(IsPlayerDead)
	{
        CreateLoseWidget();
	}
}

void AHorrorPrototypeGameModeBase::CreateLoseWidget()
{
    if (UILoseWidget != nullptr) {
        LoseWidget = CreateWidget<UUserWidget>(mainCharacterController, UILoseWidget);
        LoseWidget->AddToViewport();
        //CurrentWidget = PauseWidget;
   	
        mainCharacterController->inputModeGameAndUI.SetHideCursorDuringCapture(true);
        mainCharacterController->inputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        //mainCharacterController->inputModeGameAndUI.SetWidgetToFocus(LoseWidgetCOCACOLA->TakeWidget());

        mainCharacterController->SetInputMode(mainCharacterController->inputModeGameAndUI);

        //mainCharacter->canExecuteAction = false;

        LoseWidget->SetKeyboardFocus();
        mainCharacterController->bShowMouseCursor = true;

        UGameplayStatics::SetGamePaused(this, true);
        UE_LOG(LogTemp, Warning, TEXT("Activating pause widget"));

        /*   if (musicManager != nullptr) {
               musicManager->TurnOffGeneralVolume(necroCharacter);
           }*/
        mainCharacter->ResetCharacterParams();
    }
}

void AHorrorPrototypeGameModeBase::RemoveLoseWidget()
{
    LoseWidget->RemoveFromParent();
    LoseWidget = nullptr;
}

void AHorrorPrototypeGameModeBase::CreateCharacterWidget()
{
    if (UICharacterWidget != nullptr) {
        CharacterWidget = CreateWidget<UUserWidget>(mainCharacterController, UICharacterWidget);
        CharacterWidget->AddToViewport();
    }
}

void AHorrorPrototypeGameModeBase::ShowInventory()
{
    inventoryVisible = true;
}

void AHorrorPrototypeGameModeBase::HideInventory()
{
    inventoryVisible = false;
}

void AHorrorPrototypeGameModeBase::ShowNoAmmoMessage()
{
    noAmmoMessageVisible = true;
}

void AHorrorPrototypeGameModeBase::HideNoAmmoMessage()
{
    noAmmoMessageVisible = true;
}