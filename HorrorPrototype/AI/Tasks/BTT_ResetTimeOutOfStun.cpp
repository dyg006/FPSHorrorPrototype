// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ResetTimeOutOfStun.h"
#include "../MonsterController.h"

EBTNodeResult::Type UBTT_ResetTimeOutOfStun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<AMonsterController>(OwnerComp.GetAIOwner())->ResetTimeOutOfStun();

	return EBTNodeResult::Succeeded;
}