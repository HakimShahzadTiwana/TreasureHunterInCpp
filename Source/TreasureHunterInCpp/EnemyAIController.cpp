// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	//Create AI Components BT and BB
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Compnent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	//Start the behavior tree
	
	if(IsValid(BehaviorTree.Get()))
	{
		
		BehaviorTreeComponent->StartTree(*BehaviorTree.Get());
		if(BehaviorTreeComponent->IsRunning())
		{
			GEngine->AddOnScreenDebugMessage(-1,2,FColor::Orange,TEXT("Tree running"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Orange,TEXT("Behaviour Tree Not Valid"));
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//If BT and BB are set initialize the BB values.
	if(IsValid(Blackboard.Get()) && IsValid(BehaviorTree.Get()))
	{
		if(Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get()))
		{
			GEngine->AddOnScreenDebugMessage(-1,3,FColor::Orange,TEXT("Blackboard init successful"));
		}
	}
	
}



