// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetBool.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_SetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(boolKey.SelectedKeyName, boolValue);

	return EBTNodeResult::Succeeded;
}