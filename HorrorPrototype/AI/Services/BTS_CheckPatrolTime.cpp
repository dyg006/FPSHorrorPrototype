// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckPatrolTime.h"

#include "HorrorPrototype/AI/MonsterController.h"

UBTS_CheckPatrolTime::UBTS_CheckPatrolTime()
{

}

void UBTS_CheckPatrolTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementación de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenemos la referencia del monsterController
	AMonsterController* monsterController = Cast<AMonsterController>(OwnerComp.GetOwner());
	
	monsterController->patrollingTime += DeltaSeconds;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("patrollingTime: %lf"), monsterController->patrollingTime));
	
	if(monsterController->patrollingTime >= monsterController->maxPatrolTime) {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("HasToReturnToInitialPosition", true);
		monsterController->patrollingTime = 0;
		//OwnerComp.GetBlackboardComponent()->SetValueAsFloat("PatrollingTime", patrollingTime);
	}
}
