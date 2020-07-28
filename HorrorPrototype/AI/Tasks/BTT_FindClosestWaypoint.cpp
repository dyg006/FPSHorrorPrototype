// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindClosestWaypoint.h"
#include "../MonsterController.h"

EBTNodeResult::Type UBTT_FindClosestWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<AMonsterController>(OwnerComp.GetAIOwner())->FindClosestWaypoint();

	return EBTNodeResult::Succeeded;
}
