// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"
DEFINE_LOG_CATEGORY(SaveGameLogs)
UMySaveGame::UMySaveGame()
{
	ItemsCollected=0;
	PlayerLocation=FVector::ZeroVector;
	PlayerRotation=FRotator::ZeroRotator;
	
}
