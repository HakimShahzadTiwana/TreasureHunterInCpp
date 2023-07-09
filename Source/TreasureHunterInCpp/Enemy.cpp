// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyAIController.h"
#include "MyCharacter.h"
#include "MyDeveloperSettings.h"
#include "Animation/MovieSceneMarginTrack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
class UMyDeveloperSettings;

AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create PawnSensing Component (No need to to attach to root just create)
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Comp"));


}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//Get AI Controller 
	EnemyAIController = Cast<AAIController>(GetController());
	if(!IsValid(EnemyAIController))
	{
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Orange,TEXT("AIController not valid"));
	}
	//Set OnSeePawn function for when enemy sees player.
	if(IsValid(PawnSensingComponent))
	{
		PawnSensingComponent->bSeePawns=true;
		PawnSensingComponent->OnSeePawn.AddDynamic(this,&AEnemy::OnSeePawn);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Orange,TEXT("Pawn Sensing not valid"));
	}
	const UMyDeveloperSettings* DevSettings = GetDefault<UMyDeveloperSettings>();
	float EnemyWalkSpeed = DevSettings->EnemySpeed;
	if (EnemyWalkSpeed == 0 )
	{
		
		FString ConfigPath = "C:/Users/hakim/Unreal_Projects/TreasureHunterInCpp/Saved/Config/WindowsEditor/TreasureHuntConfigs.ini";
		GConfig->GetFloat(TEXT("/Script/TreasureHunterInCpp.MyEnemy"),TEXT("ConfigEnemySpeed"),ConfigEnemySpeed,*ConfigPath);
		UE_LOG(MovementLogs,Display,TEXT("Enemy Player Speed set from Config File %f"),ConfigEnemySpeed);
		GetCharacterMovement()->MaxWalkSpeed=ConfigEnemySpeed;
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Orange,TEXT("Enemy Speed Set From Config"));
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed=EnemyWalkSpeed;
		UE_LOG(MovementLogs,Display,TEXT("Enemy Player Speed set from dev settings %f"),EnemyWalkSpeed);
		
	}
	
	
	
}

void AEnemy::OnSeePawn(APawn* APawn)
{
	SetBlackBoardCanSeePlayer();
	UnsetBlackboardComponentRetriggerableDelay();
}

void AEnemy::ResetCanSeePlayerBlackBoard() const
{
	//Call to stop chasing and go back to patrol state
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer",false);
}

void AEnemy::SetBlackBoardCanSeePlayer() const
{
	//Set CanSeeVariable in blackboard to true when player is seen so it can call the chase state in BT
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer",true);
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Orange,TEXT("Setting Bloackboard Can See Player"));
}

void AEnemy::UnsetBlackboardComponentRetriggerableDelay()
{
	FLatentActionInfo LatentAction;
	LatentAction.CallbackTarget=this;
	LatentAction.ExecutionFunction="ResetCanSeePlayerBlackBoard";
	LatentAction.Linkage=0;
	//Set retriggerable delay so that if enemy doesnt sense the player for one second it stops chasing 
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(),1,LatentAction);
}