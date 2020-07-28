// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ResetPatrollingTime.h"
#include "../MonsterController.h"

EBTNodeResult::Type UBTT_ResetPatrollingTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<AMonsterController>(OwnerComp.GetAIOwner())->ResetPatrollingTime();

	return EBTNodeResult::Succeeded;
}