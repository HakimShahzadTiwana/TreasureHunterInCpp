// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Enemy.generated.h"
UCLASS(Config=TreasureHuntConfigs)
class TREASUREHUNTERINCPP_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Enemy Data",meta=(SliderExponent,UIMax=700,UIMin=200))
	float EnemySpeed=500;
private:
	UFUNCTION(BlueprintCallable)
	void OnSeePawn(APawn* APawn);

	UFUNCTION(BlueprintCallable)
	void ResetCanSeePlayerBlackBoard() const;
	/*Sets Blackboard CanSeePlayer variable to true so that decorater in behaviour tree responds to change */
	UFUNCTION(BlueprintCallable)
	void SetBlackBoardCanSeePlayer() const;
	/*Sets Blackboard CanSeePlayer variable to false so that decorater in behaviour tree responds to change */
	UFUNCTION(BlueprintCallable)
	void UnsetBlackboardComponentRetriggerableDelay();
	UPROPERTY(Config)
	float ConfigEnemySpeed;
	
	AAIController* EnemyAIController;

};
