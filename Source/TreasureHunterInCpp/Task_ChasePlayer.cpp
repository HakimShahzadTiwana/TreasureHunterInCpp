// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_ChasePlayer.h"
#include "AIController.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

UTask_ChasePlayer::UTask_ChasePlayer()
{
	NodeName=TEXT("Chase Player");
}


EBTNodeResult::Type UTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and PLayer Character, then move Enemy to Player
	AAIController* AIController {OwnerComp.GetAIOwner()};
	AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	AIController->MoveToActor(PlayerActor,0,true,true,true,0,true);
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
