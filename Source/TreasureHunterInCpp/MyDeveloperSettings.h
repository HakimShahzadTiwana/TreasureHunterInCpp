// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MyDeveloperSettings.generated.h"

/**
 * 
 */

UCLASS(Config=Game, DefaultConfig,meta = (DisplayName="Game Settings"))
class TREASUREHUNTERINCPP_API UMyDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config,EditAnywhere,BlueprintReadOnly, Category = "Speed",meta=(ClampMin=0))
	float PlayerSpeed;
	UPROPERTY(Config,EditAnywhere,BlueprintReadOnly, Category = "Speed",meta=(ClampMin=0))
	float EnemySpeed;
	UPROPERTY(Config,EditAnywhere,BlueprintReadOnly, Category = "Collectable Items",meta=(ClampMin=0))
	float WinCollectableCount;
	UMyDeveloperSettings();
};
