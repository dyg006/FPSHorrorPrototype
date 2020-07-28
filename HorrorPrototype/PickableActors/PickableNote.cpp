// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableNote.h"

// Sets default values
APickableNote::APickableNote()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMesh;

	pickableComponent = CreateDefaultSubobject<UPickableComponent>(TEXT("Pickable"));

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	boxCollision->SetupAttachment(RootComponent);

	textRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	textRender->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APickableNote::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableNote::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

