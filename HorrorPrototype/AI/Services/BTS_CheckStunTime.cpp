// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckStunTime.h"
#include "HorrorPrototype/AI/MonsterController.h"

void UBTS_CheckStunTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementación de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenemos la referencia del monsterController
	AMonsterController* monsterController = Cast<AMonsterController>(OwnerComp.GetOwner());

	monsterController->timeOutOfStun += DeltaSeconds;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("patrollingTime: %lf"), monsterController->timeOutOfStun));
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("canBeStunned: %s"), monsterController->canBeStunned ? TEXT("true") : TEXT("false")));

	if (monsterController->timeOutOfStun >= monsterController->delayBeforeNextStun) {
		monsterController->canBeStunned = true;
	}
}
