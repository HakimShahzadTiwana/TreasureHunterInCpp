// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_EnemyPatrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Tasks/BTTask_MoveDirectlyToward.h"


UTask_EnemyPatrol::UTask_EnemyPatrol()
{
	NodeName = TEXT("Find Random Location");
	BlackboardKey.AddVectorFilter(this,GET_MEMBER_NAME_CHECKED(UTask_EnemyPatrol,BlackboardKey));
}

EBTNodeResult::Type UTask_EnemyPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Orange,TEXT("Enemy Patrol Task Executed"));
	
	FNavLocation Location{};
	//Get AI Controller and Pawn Reference
	AAIController* AIController {OwnerComp.GetAIOwner()}; // Same as AIController = OwnerComp.GetAIComp, called initializer lists
	const APawn* AIPawn {AIController->GetPawn()};
	//Get the starting position of pawn
	const FVector Origin {AIPawn->GetActorLocation()};
	//Set up the navigation system and get a random point within the search radius
	if(const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()); IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin,SearchRadius,Location))
	{
		//Set Blackboard vector variable to position to move towards
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}
	//Move to that location
	AIController->MoveToLocation(Location.Location ,10,true,true,true,false,0,true);
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

FString UTask_EnemyPatrol::GetStaticDescription() const
{
	//Description of node shown in BT
	return FString::Printf(TEXT("Vector: %s "),*BlackboardKey.SelectedKeyName.ToString());
}
