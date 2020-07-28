// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GenericActors/CustomWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "HorrorPrototype/MainCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "MonsterController.generated.h"

class AMonsterCharacter;

/**
 * 
 */
UCLASS()
class HORRORPROTOTYPE_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMonsterController();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision")
	/// <summary>
	/// The close range in which the enemy can see the player
	/// </summary>
	float closeRange = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision")
	/// <summary>
	/// The far range in which the enemy can see the player
	/// </summary>
	float farRange = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vision")
	/// <summary>
	/// The field of view
	/// </summary>
	float fovAngle = 180;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFollow")
	/// <summary>
	/// The maximum astray distance in which the enemy can get lost
	/// </summary>
	float maxAstray = 500;

	/// <summary>
	/// Checks if the player is perceived.
	/// </summary>
	void CheckPlayerPerceived();

	/// <summary>
	/// Checks if the stucked on the map.
	/// </summary>
	/// <param name="StuckDistance">The stuck distance.</param>
	void CheckStucked(float StuckDistance);

	/// <summary>
	/// Asks if can attack player.
	/// </summary>
	void AskCanAttackPlayer();

	/// <summary>
	/// Checks whether or not the actor has gone to far from the path.
	/// </summary>
	/// <param name="followPositionKeyName">Name of the position to follow key.</param>
	void CheckAstrayed(FName followPositionKeyName);

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Sets the AI active.
	/// </summary>
	/// <param name="active">if set to <c>true</c> [active].</param>
	void SetAIActive(bool active);

	/// <summary>
	/// Gets the random point navigable.
	/// </summary>
	/// <param name="origin">The origin.</param>
	/// <param name="radius">The radius.</param>
	/// <returns></returns>
	FVector GetRandomPointNavigable(FVector origin, float radius);

	/// <summary>
	/// Finds the next waypoint on the patrol.
	/// </summary>
	void FindClosestWaypoint();

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The maximum patrol time
	/// </summary>
	float maxPatrolTime = 30.f;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The delay time before next stun
	/// </summary>
	float delayBeforeNextStun = 10.f;
	
	/// <summary>
	/// The patrolling time
	/// </summary>
	float patrollingTime;

	/// <summary>
	/// Resets the patrolling time.
	/// </summary>
	void ResetPatrollingTime() { patrollingTime = 0.0f; }

	/// <summary>
	/// Resets the time out of stun.
	/// </summary>
	void ResetTimeOutOfStun() { timeOutOfStun = 0.0f; }

	/*Sets the new sensed target value inside our Blackboard values*/
	void SetSensedTarget();

	//try to stun the enemy if possible
	void TryToStunEnemy();

	/// <summary>
	/// The time out of stun
	/// </summary>
	float timeOutOfStun = 0.f;

	/// <summary>
	/// Checks if the enemy can be stunned
	/// </summary>
	bool canBeStunned = false;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	/// <summary>
	/// The behavior tree
	/// </summary>
	UBehaviorTree* behaviorTree;

	/// <summary>
	/// The blackboard
	/// </summary>
	UBlackboardComponent* blackboard;

	/// <summary>
	/// The timer handle
	/// </summary>
	FTimerHandle timerHandle;

	/// <summary>
	/// The nav system
	/// </summary>
	UNavigationSystemV1* navSys;

	//MonsterManager* monsterManager;
	
	/// <summary>
	/// Sets the can attack player.
	/// </summary>
	/// <param name="canAttack">if set to <c>true</c> [can attack].</param>
	void SetCanAttackPlayer(bool canAttack);

	/// <summary>
	/// Gets the can attack player.
	/// </summary>
	/// <returns></returns>
	bool GetCanAttackPlayer();

	/// <summary>
	/// Sets the is astrayed.
	/// </summary>
	/// <param name="isAstrayed">if set to <c>true</c> [is astrayed].</param>
	void SetIsAstrayed(bool isAstrayed);

	/// <summary>
	/// Gets the is astrayed.
	/// </summary>
	/// <returns></returns>
	bool GetIsAstrayed();

	// Called when the game starts or when spawned
	void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The perception
	/// </summary>
	UAIPerceptionComponent* perception;
	
private:

	/// <summary>
	/// The previous location on the path of the AI
	/// </summary>
	FVector m_previousLocation;

	/// <summary>
	/// The waypoints that the AI will follow
	/// </summary>
	TArray<AActor*> waypoints;

	/// <summary>
	/// The next position index
	/// </summary>
	int nextPositionIndex = 0;
	
	/// <summary>
	/// The main character
	/// </summary>
	AMainCharacter* mainCharacter;

	/// <summary>
	/// The monster controlled
	/// </summary>
	AMonsterCharacter* monsterControlled;
	
	/// <summary>
	/// The sight
	/// </summary>
	UAISenseConfig_Sight* sight;

	/// <summary>
	/// The hearing
	/// </summary>
	UAISenseConfig_Hearing* hearing;

	/// <summary>
	/// The damage sense
	/// </summary>
	UAISenseConfig_Damage* damageSense;

	UFUNCTION()
	/// <summary>
	/// Called whenever an actor is detected by senses.
	/// </summary>
	/// <param name="UpdatedActors">The updated actors.</param>
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
};
