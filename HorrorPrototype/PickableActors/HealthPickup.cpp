// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMesh;

	pickableComponent = CreateDefaultSubobject<UPickableComponent>(TEXT("Pickable"));

	capsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	capsuleCollision->SetupAttachment(RootComponent);

	staticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondaryStaticMesh"));
	staticMesh2->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(GetActorRotation() + FRotator(0.f, 1.f, 0.f));
}

