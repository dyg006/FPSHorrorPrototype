// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableComponent.h"

#include "FMODBlueprintStatics.h"
#include "Kismet/GameplayStatics.h"
#include "../PickableActors/HealthPickup.h"
#include "../MainCharacter.h"

// Sets default values for this component's properties
UPickableComponent::UPickableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPickableComponent::BeginPlay()
{
	Super::BeginPlay();

	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void UPickableComponent::ExecuteInteraction()
{
	if (mainCharacter != nullptr) {
		if (typeOfItem == TypeOfItem::Health) {
			AHealthPickup* healthPickup = Cast<AHealthPickup>(GetOwner());
			if (healthPickup != nullptr) {
				if (mainCharacter->healthComponent->IsFullHealth())
				{
					UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), healthPickup->healthPickupNegationSound, healthPickup->GetActorTransform(), true);
					return;
				}
				else {
					mainCharacter->healthComponent->health = FMath::Min(mainCharacter->healthComponent->defaultHealth, healthPickup->health + mainCharacter->healthComponent->health);
					UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), healthPickup->healthPickupSound, healthPickup->GetActorTransform(), true);
					healthPickup->Destroy();
				}
			
			}

			return;
		}
		mainCharacter->AddToInventory(GetOwner(), typeOfItem);
		GetOwner()->SetActorEnableCollision(false);
	}
}