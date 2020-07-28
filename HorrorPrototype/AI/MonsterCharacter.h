// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "MonsterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FResidualDamageDelegate, float, Damage, float, MotionModifier, float, ActiveTime, AActor*, DamageCauser);

UCLASS()
class HORRORPROTOTYPE_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterCharacter();

	/*A Pawn Sensing Component, responsible for sensing other Pawns*/
	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* pawnSensingComp;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle for efficient management of OnTimerTick timer
	FTimerHandle DestroyTimerHandle;

	// Destroy original actor
	virtual void DestroyTimer(FVector Direction);

	/// <summary>
	/// The common enemy character animation
	/// </summary>
	UAnimInstance* enemyAnimationInstance;

	/// <summary>
	/// Attack anim montage of the common monster character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* attackAnimMontage;

	/// <summary>
	/// Enemy skeletal mesh component
	/// </summary>
	USkeletalMeshComponent* enemyMeshComponent = nullptr;

	/// <summary>
	/// Variable used to check if the monster has attacked
	/// </summary>
	bool hasAttacked = false;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The attack range of the enemy
	/// </summary>
	float _attackRange = 125.f;
	
public:	
	// Called every frame
	/*virtual void Tick(float DeltaTime) override;*/

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	/// <summary>
	/// The player damage
	/// </summary>
	float playerDamage;

	/** Controller that gets credit for any damage caused by this volume */
	UPROPERTY()
	class AController* DamageInstigator;

	// Damage sigle actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void CausePainTo(class AActor* enemy, bool useDistance = false);

	/// <summary>
	/// Gets the dist to enemy.
	/// </summary>
	/// <param name="enemy">The enemy.</param>
	/// <returns></returns>
	float GetDistToEnemy(AActor* enemy) const;

	UFUNCTION()
	/// <summary>
	/// Takes the damage.
	/// </summary>
	/// <param name="Damage">The damage.</param>
	/// <param name="DamageEvent">The damage event.</param>
	/// <param name="EventInstigator">The event instigator.</param>
	/// <param name="DamageCauser">The damage causer.</param>
	/// <returns></returns>
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION()
	/// <summary>
	/// Called when [hit].
	/// </summary>
	/// <param name="SelfActor">The self actor.</param>
	/// <param name="OtherActor">The other actor.</param>
	/// <param name="NormalImpulse">The normal impulse.</param>
	/// <param name="Hit">The hit.</param>
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	/// <summary>
	/// Attacks this instance.
	/// </summary>
	void Attack(AActor* enemyToAttack);

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	/// <summary>
	/// The objective to attack
	/// </summary>
	AActor* attackObjective;

	UPROPERTY(BlueprintAssignable, Category = "Test")
	/// <summary>
	/// The residual damage event
	/// </summary>
	FResidualDamageDelegate ResidualDamageEvent;
	
	/// <summary>
	/// The timer handle residual damage timer
	/// </summary>
	FTimerHandle TimerHandle_ResidualDamageTimer;
	
	/// <summary>
	/// The timer handle receive damage timer
	/// </summary>
	FTimerHandle TimerHandle_ReceiveDamageTimer;
	
	/// <summary>
	/// The residual damage amount
	/// </summary>
	float ResidualDamageAmount;
	
	/// <summary>
	/// The speed modifier
	/// </summary>
	float SpeedModifier;
	
	/// <summary>
	/// The is speed modified
	/// </summary>
	bool isSpeedModified;
	
	/// <summary>
	/// The has residual damage
	/// </summary>
	bool hasResidualDamage;

	//The character movement component of the enemy
	class UCharacterMovementComponent* _characterMovement;
	
	/// <summary>
	/// The residual damage causer
	/// </summary>
	AActor* ResidualDamageCauser;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	/// <summary>
	/// Starts the residual damage.
	/// </summary>
	/// <param name="Damage">The damage.</param>
	/// <param name="MotionModifier">The motion modifier.</param>
	/// <param name="ActiveTime">The active time.</param>
	/// <param name="DamageCauser">The damage causer.</param>
	void StartResidualDamage(float Damage, float MotionModifier, float ActiveTime, AActor* DamageCauser);
	
	/// <summary>
	/// Stops the residual damage.
	/// </summary>
	void StopResidualDamage();
	
	/// <summary>
	/// Makes the residual damage.
	/// </summary>
	void MakeResidualDamage();

	/// <summary>
	/// Sets the character speed.
	/// </summary>
	/// <param name="speed">The speed.</param>
	void SetCharacterSpeed(float speed);

private:
	UFUNCTION()
	/// <summary>
	/// Called when [hear noise].
	/// </summary>
	/// <param name="PawnInstigator">The pawn instigator.</param>
	/// <param name="Location">The location.</param>
	/// <param name="Volume">The volume.</param>
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	/// <summary>
	/// The headshot multiplier factor used to increase the damage that the enemy receives from headshots
	/// </summary>
	float headshotMultiplierFactor = 2.f;
};
