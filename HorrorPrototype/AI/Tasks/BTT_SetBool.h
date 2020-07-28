// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetBool.generated.h"

/**
 * 
 */
UCLASS()
class HORRORPROTOTYPE_API UBTT_SetBool : public UBTTaskNode
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
	/// The blackboard bool key to set
	/// </summary>
	FBlackboardKeySelector boolKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	/// <summary>
	/// The bool value to set on the blackboard variable
	/// </summary>
	bool boolValue;
};