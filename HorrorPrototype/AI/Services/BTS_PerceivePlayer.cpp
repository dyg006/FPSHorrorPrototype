// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PerceivePlayer.h"
#include "../MonsterController.h"

UBTS_PerceivePlayer::UBTS_PerceivePlayer()
{

}

void UBTS_PerceivePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementaci�n de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenemos la referencia del monsterController
	AMonsterController* monsterController = Cast<AMonsterController>(OwnerComp.GetOwner());

	//Llamamos al m�todo CheckPlayerPerceived del monsterController que tiene toda la l�gica para determinar si el
	// jugador est� cerca o no y configurar el Key correspontiente del Blackboard
	monsterController->CheckPlayerPerceived();
}
