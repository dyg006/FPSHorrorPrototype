// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Projectile.generated.h"

UCLASS()
class HORRORPROTOTYPE_API AWeapon_Projectile : public AActor
{
	GENERATED_BODY()
	
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	AWeapon_Projectile();

	UPROPERTY(EditAnywhere)
	//The static mesh of the actor
	class UStaticMeshComponent* staticMesh;
	
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	/// <summary>
	/// Causes the pain to an actor.
	/// </summary>
	/// <param name="enemy">The enemy.</param>
	/// <param name="Hit">The hit.</param>
	void CausePainTo(AActor* enemy, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The damage that the projectile deals by default
	/// </summary>
	float damage = 10.f;
};
