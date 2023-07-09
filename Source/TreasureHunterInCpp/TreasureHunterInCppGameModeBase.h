// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "MySaveGame.h"

#include "GameFramework/GameModeBase.h"
#include "Components/TextBlock.h"
#include "TreasureHunterInCppGameModeBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FGameOverDelegate,AMyTreasureItem*,TreasureItem);

DECLARE_LOG_CATEGORY_EXTERN(GameModeLogs, Display, All);

UCLASS(Config=TreasureHuntConfigs)
class TREASUREHUNTERINCPP_API ATreasureHunterInCppGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATreasureHunterInCppGameModeBase();
	
	UFUNCTION(BlueprintCallable)
	void WinGame() const;

	UFUNCTION(BlueprintCallable)
	void IncrementScore(AMyTreasureItem* TreasureItem);
	
	UFUNCTION()
	void GameOver() const;
	
	/*Creates Main Display widget for score and end game text*/
	UFUNCTION(BlueprintCallable)
	void CreateMainWidget();
	
	/*Adds Main widget to view port*/
	UFUNCTION(BlueprintCallable)
	void AddWidgetToViewPort() const;
	
	/*Counts total treasure items in level and returns sets the count to totalCount variable*/
	UFUNCTION(BlueprintCallable)
	void SetTotalItems();

	UFUNCTION()
	void SaveGame(int index);

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void LoadDLC();
	
	UPROPERTY()
	TArray<AMyTreasureItem*> ItemsInLevel;
	
	UPROPERTY()
	FGameOverDelegate Delegate_GameOver;
	
	UPROPERTY()
	FGameOverDelegate Delegate_UpdateScore;

	UPROPERTY()
	UMySaveGame* SavedGameInstance;

	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> WidgetClass;
	
	UPROPERTY()
	UUserWidget* widget;
	
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* txtScore;
	
	UPROPERTY()
	UTextBlock* txtGameEnd;
	
	UPROPERTY(BlueprintReadOnly)
	int score;
	
	UPROPERTY(Config)
	int totalItems;
	
	UPROPERTY()
	AMyCharacter* PlayerCharacter;


	
};
