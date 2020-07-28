// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController.h"


#include "MonsterCharacter.h"
#include "HorrorPrototype/HorrorPrototypeGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AMonsterController::AMonsterController()
{
	//Perception configuration
	perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));

	sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
	damageSense = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damaging"));

	//We set up all senses configuration
	perception->ConfigureSense(*sight);
	perception->ConfigureSense(*hearing);
	perception->ConfigureSense(*damageSense);
	perception->SetDominantSense(sight->GetSenseImplementation());

	//perception->OnPerceptionUpdated.AddDynamic(this, &AAIController::ActorsPerceptionUpdated);
	perception->OnPerceptionUpdated.AddDynamic(this, &AMonsterController::ActorsPerceptionUpdated);
}

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();
	
	if (behaviorTree == nullptr)
	{
		return;
	}
	
	RunBehaviorTree(behaviorTree);
	
	blackboard = GetBlackboardComponent();
	
	if (blackboard == nullptr)
	{
		return;
	}
	blackboard->SetValueAsBool("IsDeactivated", false);
	blackboard->SetValueAsBool("CanAttackPlayer", true);
	blackboard->SetValueAsBool("IsLost", false);

	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	blackboard->SetValueAsObject("EnemyActor", mainCharacter);
	

	navSys = UNavigationSystemV1::GetCurrent(GetWorld());

	m_previousLocation = FVector::ZeroVector;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomWaypoint::StaticClass(), waypoints);

	monsterControlled = Cast<AMonsterCharacter>(GetPawn());

	blackboard->SetValueAsVector("InitialPosition", monsterControlled->GetActorLocation());
	blackboard->SetValueAsRotator("InitialRotation", monsterControlled->GetActorRotation());
}

void AMonsterController::SetCanAttackPlayer(bool canAttack)
{
	blackboard->SetValueAsBool("CanAttackPlayer", canAttack);
}

bool AMonsterController::GetCanAttackPlayer()
{
	return blackboard->GetValueAsBool("CanAttackPlayer");
}

void AMonsterController::SetIsAstrayed(bool isAstrayed)
{
	blackboard->SetValueAsBool("IsAstrayed", isAstrayed);
}


bool AMonsterController::GetIsAstrayed()
{
	return blackboard->GetValueAsBool("IsAstrayed");
}

void AMonsterController::SetAIActive(bool active)
{
	blackboard->SetValueAsBool("IsDeactivated", !active);
}


void AMonsterController::CheckPlayerPerceived()
{
	AHorrorPrototypeGameModeBase* gm = Cast<AHorrorPrototypeGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		if (GetIsAstrayed() || gm->GetIsPlayerDead())
		{
			blackboard->SetValueAsBool("IsPlayerPerceived", false);
			blackboard->SetValueAsBool("IsPlayerClose", false);
			return;
		}
	}

	AActor* player = Cast<AActor>(blackboard->GetValueAsObject("EnemyActor"));

	FVector direction = player->GetActorLocation() - GetPawn()->GetActorLocation();

	float distance = direction.Size();

	bool canAttackByDistance;

	if (distance > farRange)
	{
		canAttackByDistance = false;
		blackboard->SetValueAsBool("IsPlayerClose", false);
	}
	else if (distance < closeRange)
	{
		canAttackByDistance = true;
		blackboard->SetValueAsBool("IsPlayerClose", true);
	}
	else
	{
		FVector lookingAt = GetPawn()->GetViewRotation().Vector();

		// angle = acos( (a · b) / (|a| * |b|))
		float angle = FMath::Acos(
			FVector::DotProduct(direction, lookingAt)
			/ (direction.Size() * lookingAt.Size()));

		angle = (angle / PI) * 180.f;

		canAttackByDistance = angle < fovAngle / 2.f;
		blackboard->SetValueAsBool("IsPlayerClose", true);
	}

	blackboard->SetValueAsBool("IsPlayerPerceived", canAttackByDistance);

}

void AMonsterController::CheckStucked(float StuckDistance)
{
	FVector ownLocation = GetPawn()->GetActorLocation();

	if (!m_previousLocation.Equals(FVector::ZeroVector))
	{
		// check if we are in the same location
		if (FVector::Distance(ownLocation, m_previousLocation) < StuckDistance)
		{
			blackboard->SetValueAsBool("IsStucked", true);
		}
		else
		{
			blackboard->SetValueAsBool("IsStucked", false);
		}
	}

	m_previousLocation = ownLocation;
}

void AMonsterController::AskCanAttackPlayer()
{
	if (monsterControlled->HealthComponent->alive) {
		//bool canAttackPlayer = true; // TryAttackPlayer(GetPawn()->GetClass());
		SetCanAttackPlayer(true);
	} else {
		SetCanAttackPlayer(false);
	}

}


/// <summary>
/// Checks the astrayed.
/// </summary>
/// <param name="followPositionKeyName">Name of the follow position key.</param>
void AMonsterController::CheckAstrayed(FName followPositionKeyName)
{
	FVector targetPosition = blackboard->GetValueAsVector(followPositionKeyName);
	FVector ownPosition = GetPawn()->GetActorLocation();
	float distance = (targetPosition - ownPosition).Size();

	if (distance >= maxAstray) {
		SetIsAstrayed(true);
		blackboard->SetValueAsBool("IsRecentlyAstrayed", true);
	}

}


FVector AMonsterController::GetRandomPointNavigable(FVector origin, float radius)
{
	FNavLocation result;
	navSys->GetRandomPointInNavigableRadius(origin, radius, result);
	return result.Location;
}

void AMonsterController::FindClosestWaypoint()
{
	if (waypoints.Num() > 0)
	{
		int indexWithCloserPoint = nextPositionIndex;
		nextPositionIndex = (nextPositionIndex + 1) % waypoints.Num(); //We get the next position in the circular array
		blackboard->SetValueAsObject("NextWaypoint", waypoints[indexWithCloserPoint]);
		/*float minDistance = 0.0f;
		for (int i = 0; i < waypoints.Num(); ++i) {
			if (i == 0) {
				minDistance = mainCharacter->GetDistanceTo(waypoints[0]);
			}
			else {
				if (mainCharacter->GetDistanceTo(waypoints[i]) < minDistance) {
					minDistance = mainCharacter->GetDistanceTo(waypoints[i]);
					indexWithCloserPoint = i;
				}
			}
		}*/
		
	}
}

void AMonsterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	for(AActor* perceivedActor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo info;
		bool wasSensed = false;
		perception->GetActorsPerception(perceivedActor, info);
		if (info.LastSensedStimuli.Num() > 0) {
			wasSensed = info.LastSensedStimuli[0].WasSuccessfullySensed();
		}
		float distanceToEnemy = GetPawn()->GetDistanceTo(Cast<AMainCharacter>(blackboard->GetValueAsObject("EnemyActor")));
		if(wasSensed) {
			if(!blackboard->GetValueAsBool("IsRunning")) {
				if (UpdatedActors.Num() >= 1) {
					blackboard->SetValueAsBool("IsPlayerPerceivedBySenses", true);
				}
			}
			
		} else {
			blackboard->SetValueAsBool("IsPlayerPerceivedBySenses", false);
		}
	}
}

void AMonsterController::SetSensedTarget()
{
	//Set a new target to follow
	if (blackboard != nullptr) {
		blackboard->SetValueAsBool("IsPlayerPerceivedBySenses", true);
	}
}

void AMonsterController::TryToStunEnemy()
{
	if (blackboard != nullptr && canBeStunned) {
		blackboard->SetValueAsBool("IsStunned", true);
		ResetTimeOutOfStun();
		canBeStunned = false;
	}
}