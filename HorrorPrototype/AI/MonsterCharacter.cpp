// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"
#include "MonsterController.h"
#include "../MainCharacter.h" 
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->Init(100);

	pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(70);
	GetCapsuleComponent()->SetCapsuleRadius(50);

	// Rotate mesh to follow ArrowComponent
	FRotator NewRotation = FRotator(0.f, 0.f, -90.f);
	FQuat QuatRotation = FQuat(NewRotation);
	GetMesh()->SetWorldRotation(QuatRotation);

	enemyMeshComponent = GetMesh();

	isSpeedModified = false;
	hasResidualDamage = false;
	ResidualDamageEvent.AddDynamic(this, &AMonsterCharacter::StartResidualDamage);
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AMonsterCharacter::OnHit);

	//Registering the delegate which will fire when we hear something
	pawnSensingComp->OnHearNoise.AddDynamic(this, &AMonsterCharacter::OnHearNoise);
}

////// Called every frame
//void AMonsterCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Velocidad de la IA: %lf de vida"), GetCharacterMovement()->MaxWalkSpeed));
//}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMonsterCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	//float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	float ActualDamage = Damage;

	
	
	// DamageType example
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();

	// Health component
	if (ActualDamage != 0.f)
	{
		FPointDamageEvent* const oPointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		
		FName boneHit = oPointDamageEvent->HitInfo.BoneName;
		
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Golpe en: %s"), boneHit.ToString()));
		if (boneHit.IsEqual(TEXT("Head"))) {
			AMonsterController* monsterController = Cast<AMonsterController>(GetController());

			monsterController->TryToStunEnemy();
			
			ActualDamage *= headshotMultiplierFactor;
		}
		// Apply the damage amount
		HealthComponent->Damage(ActualDamage);

		// if dead destroy
		if (!HealthComponent->alive) {
			// if controller is not null in order to organize AIs we could have a register/unregister system for them
			//if (monsterController != nullptr) {
				/*monsterController->UnregisterFromPlayerSlot();*/
			//}

			/*if (gameMode != nullptr) {
				gameMode->IncreaseScore();
			}*/

			// generic death
			Destroy();

		}
	}

	return ActualDamage;
}

void AMonsterCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	TSubclassOf<UDamageType> DmgTypeClass = UDamageType::StaticClass();
	FVector ShotDirection(0.f, 0.f, 0.f);
	FPointDamageEvent DamageEvent = FPointDamageEvent(200, Hit, ShotDirection, DmgTypeClass);
	if (ResidualDamageCauser != nullptr) {
		TakeDamage(HealthComponent->health, DamageEvent, ResidualDamageCauser->GetInstigatorController(), ResidualDamageCauser);
	}
}

void AMonsterCharacter::CausePainTo(AActor* enemy, bool useDistance)
{

	if (playerDamage <= 0.f)
	{
		return;
	}
	if (useDistance && GetDistToEnemy(enemy) > _attackRange)
	{
		return;
	}

	// Create a damage event  
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	AController* damageInstigatorController = GetInstigatorController();
	enemy->TakeDamage(playerDamage, DamageEvent, damageInstigatorController, this);

}

float AMonsterCharacter::GetDistToEnemy(AActor* enemy) const
{
	float distance = FVector::Dist(enemy->GetActorLocation(), GetActorLocation());
	distance -= Cast<AMainCharacter>(enemy)->GetCapsuleComponent()->GetScaledCapsuleRadius();
	return distance;
}

void AMonsterCharacter::DestroyTimer(FVector Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage CPP Direction: %s"), *Direction.ToString());

	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);

	Destroy();
}

void AMonsterCharacter::Attack(AActor* objective) {
	if (enemyMeshComponent != nullptr) {
		enemyAnimationInstance = enemyMeshComponent->GetAnimInstance();

		if (!enemyAnimationInstance->Montage_IsPlaying(attackAnimMontage)) {
			attackObjective = objective;
			enemyAnimationInstance->Montage_Play(attackAnimMontage);
		}
	}
}


/*
	Control daño residual
*/
void AMonsterCharacter::StartResidualDamage(float Damage, float MotionModifier, float ActiveTime, AActor* DamageCauser)
{
	if (!hasResidualDamage)
	{
		ResidualDamageAmount = Damage;
		SpeedModifier = MotionModifier;
		ResidualDamageCauser = DamageCauser;

		hasResidualDamage = true;

		if (SpeedModifier != 0 && !isSpeedModified)
		{
			GetCharacterMovement()->MaxWalkSpeed *= SpeedModifier;
			isSpeedModified = true;
		}

		GetWorldTimerManager().SetTimer(TimerHandle_ResidualDamageTimer, this, &AMonsterCharacter::StopResidualDamage, ActiveTime, false);

		if (!GetWorldTimerManager().IsTimerActive(TimerHandle_ReceiveDamageTimer))
			GetWorldTimerManager().SetTimer(TimerHandle_ReceiveDamageTimer, this, &AMonsterCharacter::MakeResidualDamage, 1.f, true);
	}
}

void AMonsterCharacter::StopResidualDamage()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ReceiveDamageTimer);
	GetWorldTimerManager().ClearTimer(TimerHandle_ResidualDamageTimer);
	if (SpeedModifier != 0)
	{
		GetCharacterMovement()->MaxWalkSpeed /= SpeedModifier;
		isSpeedModified = false;
	}

	hasResidualDamage = false;
}

void AMonsterCharacter::MakeResidualDamage()
{
	if (ResidualDamageAmount > 0.f)
	{
		UE_LOG(LogTemp, Error, TEXT("TakeDamage by trap: %f"), ResidualDamageAmount);
		TSubclassOf<UDamageType> DmgTypeClass = nullptr;
		FHitResult Hit;
		FVector ShotDirection(0.f, 0.f, 1.f);
		FPointDamageEvent DamageEvent = FPointDamageEvent(ResidualDamageAmount, Hit, ShotDirection, DmgTypeClass);
		TakeDamage(ResidualDamageAmount, DamageEvent, this->GetInstigatorController(), ResidualDamageCauser);
	}
}

void AMonsterCharacter::SetCharacterSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void AMonsterCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	AMonsterController* controller = Cast<AMonsterController>(GetController());
	//We don't want to hear ourselves
	if (controller && PawnInstigator != this)
	{
		//Updates our target based on what we've heard.
		controller->SetSensedTarget();
	}
}