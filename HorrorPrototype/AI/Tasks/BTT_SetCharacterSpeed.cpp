// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetCharacterSpeed.h"
#include "AIController.h"
#include "../MonsterCharacter.h"

EBTNodeResult::Type UBTT_SetCharacterSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn())->SetCharacterSpeed(speed);

	return EBTNodeResult::Succeeded;
}