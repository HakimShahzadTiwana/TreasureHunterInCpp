// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTERINCPP_API UTask_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_ChasePlayer();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
