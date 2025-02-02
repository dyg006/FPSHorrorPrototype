// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyActor.h"

// Sets default values
AKeyActor::AKeyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMesh;

	pickableComponent = CreateDefaultSubobject<UPickableComponent>(TEXT("Pickable"));
	//openableComponent->SetupAttachment(RootComponent);

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	boxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

