// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckStunTime.generated.h"

/**
 * 
 */
UCLASS()
class HORRORPROTOTYPE_API UBTS_CheckStunTime : public UBTService
{
	GENERATED_BODY()
	
	/// <summary>
	/// Ticks the node.
	/// </summary>
	/// <param name="OwnerComp">The owner comp.</param>
	/// <param name="NodeMemory">The node memory.</param>
	/// <param name="DeltaSeconds">The delta seconds.</param>
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
