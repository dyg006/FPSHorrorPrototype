// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "HorrorPrototype/AI/MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon_Projectile::AWeapon_Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AWeapon_Projectile::OnHit);		// set up a notification for when this component hits something blocking
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	staticMesh->SetupAttachment(RootComponent);
	
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AWeapon_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) /*&& OtherComp->IsSimulatingPhysics()*/)
	{
		AMonsterCharacter* enemy = Cast<AMonsterCharacter>(OtherActor);
		if(enemy != nullptr) {
			CausePainTo(enemy, Hit);	
		}
	}
	Destroy();
}

void AWeapon_Projectile::CausePainTo(AActor* enemy, const FHitResult& Hit)
{
	if (damage <= 0.f)
	{
		return;
	}

	// Create a damage event
	//TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FPointDamageEvent DamageEvent;
	DamageEvent.HitInfo = Hit;
	AController* damageInstigatorController = GetInstigatorController();
	enemy->TakeDamage(damage, DamageEvent, damageInstigatorController, this);

}