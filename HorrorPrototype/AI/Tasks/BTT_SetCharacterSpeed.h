// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetCharacterSpeed.generated.h"

/**
 * 
 */
UCLASS()
class HORRORPROTOTYPE_API UBTT_SetCharacterSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Executes the task.
	/// </summary>
	/// <param name="OwnerComp">The owner comp.</param>
	/// <param name="NodeMemory">The node memory.</param>
	/// <returns>Returns Succeded, Failed or In Progress</returns>
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	/// <summary>
	/// The speed to set on the enemy
	/// </summary>
	float speed;
};
