// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetFloat.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_SetFloat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(floatKey.SelectedKeyName, floatValue);

	return EBTNodeResult::Succeeded;
}