// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "HorrorPrototypeGameModeBase.h"

AMainCharacterController::AMainCharacterController()
{
	perceptionSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerception Source Component"));
}

void AMainCharacterController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	currentPawn = this->GetPawn();
	mainCharacter = Cast<AMainCharacter>(currentPawn);
	mainCharacterMovement = mainCharacter->GetCharacterMovement();
	gameMode = Cast<AHorrorPrototypeGameModeBase>(GetWorld()->GetAuthGameMode());
	
	StopRunning();

	perceptionSourceComponent->bAutoRegister = true;
}

// Called to bind functionality to input
void AMainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("Init Input"));

	InputComponent->BindAxis("MoveForward", this, &AMainCharacterController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMainCharacterController::MoveRight);

	InputComponent->BindAxis("TurnRate", this, &AMainCharacterController::TurnAtRate);
	InputComponent->BindAxis("LookUpRate", this, &AMainCharacterController::LookUpAtRate);
	InputComponent->BindAxis("Turn", this, &AMainCharacterController::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AMainCharacterController::AddControllerPitchInput);

	InputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacterController::Shoot);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacterController::Reload);
	//InputComponent->BindAction("ReloadAction", IE_Released, this, &AMainCharacterController::EndReload);
	InputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacterController::StartRunning);
	InputComponent->BindAction("Run", IE_Released, this, &AMainCharacterController::StopRunning);
	InputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacterController::Pause).bExecuteWhenPaused = true;

	InputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacterController::StartAiming);
	InputComponent->BindAction("Aim", IE_Released, this, &AMainCharacterController::StopAiming);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacterController::StartCrouching);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacterController::Interact);
	
	//InputComponent->BindAction("ChangeToRightAsset", IE_Released, this, &AMainCharacterController::ChangeToRightAsset);
	//InputComponent->BindAction("ChangeToLeftAsset", IE_Released, this, &AMainCharacterController::ChangeToLeftAsset);
	//InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &AMainCharacterController::MeleeAttack);
	//InputComponent->BindAction("ChangeInventory", IE_Released, this, &AMainCharacterController::ChangeInventory);
	//InputComponent->BindAction("GetItem", IE_Pressed, this, &AMainCharacterController::GetItem);

}

void AMainCharacterController::MoveForward(float value)
{
	if (value != 0.0f && currentPawn != nullptr)
	{
		// find out which way is forward
		//const FRotator Rotation = this->GetControlRotation();

		// Obtenemos la rotación de la cámara en vez de el personaje
		const FRotator Rotation = this->PlayerCameraManager->GetCameraRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		currentPawn->AddMovementInput(Direction, sprintIsActive ? value / abs(value) : value);
	}
}

void AMainCharacterController::MoveRight(float value)
{
	if (value != 0.0f && currentPawn != nullptr)
	{
		// find out which way is right
		//const FRotator Rotation = this->GetControlRotation();

		// Obtenemos la rotación de la cámara en vez de el personaje
		const FRotator Rotation = this->PlayerCameraManager->GetCameraRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		currentPawn->AddMovementInput(Direction, sprintIsActive ? value / abs(value) : value);

	}
}

void AMainCharacterController::TurnAtRate(float Rate)
{
	if (currentPawn != nullptr) {
		// calculate delta for this frame from the rate information
		Rate = (FMath::Abs(Rate) < 0.5) ? Rate * 0.5 : Rate;
		currentPawn->AddControllerYawInput(Rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMainCharacterController::LookUpAtRate(float Rate)
{
	if (currentPawn != nullptr) {

		if (canLookUp) {
			// calculate delta for this frame from the rate information
			Rate = (FMath::Abs(Rate) < 0.5) ? Rate * 0.5 : Rate;
			currentPawn->AddControllerPitchInput(Rate * baseLookUpRate * GetWorld()->GetDeltaSeconds());
		}
	}
}

void AMainCharacterController::AddControllerPitchInput(float Val)
{
	if (currentPawn != nullptr) {
		currentPawn->AddControllerPitchInput(Val * mouseSensitivity);
	}
}

void AMainCharacterController::AddControllerYawInput(float Val)
{
	if (currentPawn != nullptr) {
		currentPawn->AddControllerYawInput(Val * mouseSensitivity);
	}
}

void AMainCharacterController::StartRunning()
{	
	UE_LOG(LogTemp, Warning, TEXT("StartRunning"));
	if (mainCharacter == nullptr || mainCharacterMovement == nullptr) {
		return;
	}
	if (mainCharacter->GetIsCrouching()) { // if character is crouching stop it
		mainCharacter->Crawl();
	}
	sprintIsActive = true;
	mainCharacterMovement->MaxWalkSpeed = mainCharacter->runnningSpeed;	
}

void AMainCharacterController::StopRunning()
{
	UE_LOG(LogTemp, Warning, TEXT("StopRunning"));
	sprintIsActive = false;
	mainCharacterMovement->MaxWalkSpeed = mainCharacter->walkingSpeed;
}

void AMainCharacterController::Shoot()
{
	if (mainCharacter == NULL) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to MainCharacter in MainAction"));
		return;
	}
	mainCharacter->Shoot();
}

void AMainCharacterController::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Aim"));
	if (mainCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to MainCharacter in Reload"));
		return;
	}
	mainCharacter->ReloadAction();
}

void AMainCharacterController::Pause()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause"));
	if (gameMode != nullptr)
	{
		gameMode->CreatePauseMenu();
	}
}

void AMainCharacterController::StartAiming()
{
	if (mainCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to MainCharacter in Aim"));
		return;
	}
	if (mainCharacter->canExecuteAction) {
		UE_LOG(LogTemp, Warning, TEXT("StartAiming"));
		mainCharacter->StartAiming();
	}
}

void AMainCharacterController::StopAiming()
{
	if (mainCharacter == nullptr) {
		UE_LOG(LogTemp, Error, TEXT(" Error casting from pawn to MainCharacter in Aim"));
		return;
	}
	if (mainCharacter->canExecuteAction) {
		UE_LOG(LogTemp, Warning, TEXT("StopAiming"));
		mainCharacter->StopAiming();
	}
}

void AMainCharacterController::StartCrouching()
{
	if (mainCharacter != nullptr) {
		if (mainCharacter->canExecuteAction) {
			UE_LOG(LogTemp, Warning, TEXT("StartCrouching"));
			mainCharacter->Crawl();
		}
	}
}

//void AMainCharacterController::StopCrouching()
//{
//	if (mainCharacter != nullptr) {
//		if (mainCharacter->canExecuteAction) {
//			UE_LOG(LogTemp, Warning, TEXT("StopCrouching"));
//			//mainCharacter->Crouch();		
//		}
//	}
//}

void AMainCharacterController::Interact()
{
	if (mainCharacter != nullptr) {
		if (mainCharacter->canExecuteAction) {
			UE_LOG(LogTemp, Warning, TEXT("Interact"));
			mainCharacter->Interact();
		}
	}
}