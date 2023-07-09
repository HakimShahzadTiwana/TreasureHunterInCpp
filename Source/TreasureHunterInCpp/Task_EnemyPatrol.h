// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_EnemyPatrol.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTERINCPP_API UTask_EnemyPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UTask_EnemyPatrol();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Search", meta = (AllowPrivateAccess= true))
	float SearchRadius = 500.0f;
	
};
