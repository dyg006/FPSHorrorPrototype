// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"


#include "HorrorPrototype/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMesh;

	openableComponent = CreateDefaultSubobject<UOpenableComponent>(TEXT("Openable"));

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	boxCollision->SetupAttachment(RootComponent);

	boxCollision->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ADoor::NotifyActorBeginOverlap(AActor* otherActor)
//{
//	Super::NotifyActorBeginOverlap(otherActor);
//	
//	if (mainCharacter == otherActor) {
//		UE_LOG(LogTemp, Warning, TEXT("Entrando al trigger"));
//	}
//}

void ADoor::NotifyActorEndOverlap(AActor* otherActor)
{
	Super::NotifyActorEndOverlap(otherActor);
	
	if (openableComponent != nullptr && mainCharacter != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Saliendo del trigger"));
		if (otherActor == mainCharacter)
		{
			openableComponent->SetInteracted(false);
		}
	}
}

