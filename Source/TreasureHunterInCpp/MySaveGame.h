// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(SaveGameLogs,Display,All);
UCLASS()
class TREASUREHUNTERINCPP_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMySaveGame();

	UPROPERTY()
	int ItemsCollected;
	UPROPERTY()
	FVector PlayerLocation;
	UPROPERTY()
	FRotator PlayerRotation;
	UPROPERTY()
	TArray<int> DestroyedItemIds;
};
