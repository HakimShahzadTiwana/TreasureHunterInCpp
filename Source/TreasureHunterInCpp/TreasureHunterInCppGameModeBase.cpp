// Copyright Epic Games, Inc. All Rights Reserved.


#include "TreasureHunterInCppGameModeBase.h"
#include <string>
#include "IPlatformFilePak.h"
#include "MySaveGame.h"
#include "MyTreasureItem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"

DEFINE_LOG_CATEGORY(GameModeLogs);
ATreasureHunterInCppGameModeBase::ATreasureHunterInCppGameModeBase()
{
	
}

void ATreasureHunterInCppGameModeBase::BeginPlay()
{
	
	Super::BeginPlay();
	score=0;
	if(!IConsoleManager::Get().IsNameRegistered(TEXT("CVar_TotalItems")))
	{
		IConsoleManager::Get().RegisterConsoleVariable(TEXT("CVar_TotalItems"),5,TEXT("Defines the total number of items to be collected to win the game."));
	}	
	PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	Delegate_GameOver.BindUFunction(this,TEXT("GameOver"));
	Delegate_UpdateScore.BindUFunction(this,TEXT("IncrementScore"));

	//Adding delay so we can count actors after the game loads fully
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&ATreasureHunterInCppGameModeBase::LoadGame,1.0f,false);
}

void ATreasureHunterInCppGameModeBase::WinGame() const
{
	//If the score is equal to the total items, game is won so pause game and show game won text
	if(score==totalItems)
	{
		UGameplayStatics::SetGamePaused(GetWorld(),true);
		txtGameEnd->SetText(FText::FromString("You Won!"));
		if(UGameplayStatics::DoesSaveGameExist(TEXT("Slot1"),0))
		{
			UGameplayStatics::DeleteGameInSlot(TEXT("Slot1"),0);
		}
	
	}
}

void ATreasureHunterInCppGameModeBase::IncrementScore(AMyTreasureItem* TreasureItem)
{
	//Increment the score and refresh the score widget
	UE_LOG(GameModeLogs,Display,TEXT("Increment Score was called for item id : %d"),TreasureItem->ItemId);
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,TEXT("Called Increment Score From Delegate"));
	score++;
	txtScore->SetText(FText::FormatOrdered(FText::FromString("Score : {score}/{totalItems}"),score,totalItems));
	
	SaveGame(TreasureItem->ItemId);
	WinGame();
}

void ATreasureHunterInCppGameModeBase::GameOver() const
{
	
	UGameplayStatics::SetGamePaused(GetWorld(),true);
	txtGameEnd->SetText(FText::FromString("Game Over!"));
	if(UGameplayStatics::DoesSaveGameExist(TEXT("Slot1"),0))
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("Slot1"),0);
	}
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,"GameOver Called by delegate");

}

void ATreasureHunterInCppGameModeBase::CreateMainWidget()
{
	widget = CreateWidget<UUserWidget>(GetGameInstance(),WidgetClass);
	txtScore = static_cast<UTextBlock*>(widget->GetWidgetFromName(TEXT("txtScore")));
	txtGameEnd= static_cast<UTextBlock*>(widget->GetWidgetFromName(TEXT("txtGameEnd")));
	txtGameEnd->SetText(FText::FromString(TEXT("")));
}

void ATreasureHunterInCppGameModeBase::AddWidgetToViewPort() const
{
	widget->AddToViewport();
}

 void ATreasureHunterInCppGameModeBase::SetTotalItems()
{
	
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green,TEXT("SetTotalItemCount was Called"));
		TArray<AActor*> AItems;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMyTreasureItem::StaticClass(),AItems);
		totalItems=AItems.Num();
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green,FString::Printf(TEXT("Number of items were counted %d"),AItems.Num()));
		
		for(int i=0; i<totalItems;i++)
		{
			AMyTreasureItem* TreasureItem=Cast<AMyTreasureItem>(AItems[i]);
			ItemsInLevel.Add(TreasureItem);
			TreasureItem->ItemId =i;
			UE_LOG(GameModeLogs,Display,TEXT("(Set total items) Item Name : %s Item id : %d"),*TreasureItem->GetName(),TreasureItem->ItemId);
		}
	
}

void ATreasureHunterInCppGameModeBase::SaveGame(int ItemId)
{	SavedGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if(IsValid(SavedGameInstance))
	{
		if(UGameplayStatics::DoesSaveGameExist(TEXT("Slot1"),0))
		{
			SavedGameInstance=Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"),0));
		}
		SavedGameInstance->PlayerLocation = PlayerCharacter->GetActorLocation();
		SavedGameInstance->PlayerRotation=PlayerCharacter->GetActorRotation();
		SavedGameInstance->ItemsCollected = score;
		SavedGameInstance->DestroyedItemIds.Add(ItemId);
		UGameplayStatics::SaveGameToSlot(SavedGameInstance,TEXT("Slot1"),0);
		UE_LOG(SaveGameLogs,Display,TEXT("Game was SAVED. Player Location: %s  Items Collected: %d Items Destroyed: %d"),*SavedGameInstance->PlayerLocation.ToString(),SavedGameInstance->ItemsCollected,SavedGameInstance->DestroyedItemIds.Num());
		UE_LOG(GameModeLogs,Display,TEXT("Destroyed Item index added %d"),ItemId);
		for(int id : SavedGameInstance->DestroyedItemIds)
		{
			UE_LOG(GameModeLogs,Display,TEXT("Destroyed Ids are : %d"),id);
		}
	}
	else
	{
		UE_LOG(GameModeLogs,Display,TEXT("Saved Game is Null"));
	}
}

void ATreasureHunterInCppGameModeBase::LoadGame()
{
	SetTotalItems();
	CreateMainWidget();
	AddWidgetToViewPort();
	SavedGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	SavedGameInstance =Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"),0));
	if(UGameplayStatics::DoesSaveGameExist(TEXT("Slot1"),0))
	{
		if(IsValid(PlayerCharacter))
		{
			PlayerCharacter->SetActorLocation(SavedGameInstance->PlayerLocation);
			PlayerCharacter->SetActorRotation(SavedGameInstance->PlayerRotation);
		}
		this->score = SavedGameInstance->ItemsCollected;
		for(AMyTreasureItem* Item : ItemsInLevel)
		{
			
			if(SavedGameInstance->DestroyedItemIds.Find(Item->ItemId)!= INDEX_NONE)
			{
				UE_LOG(GameModeLogs,Display,TEXT("Called for item id : %d"),Item->ItemId);
				ItemsInLevel.Remove(Item);
				Item->Destroy();
			}
		}

		UE_LOG(SaveGameLogs,Display,TEXT("Game was LOADED. Player Location: %s  Items Collected: %d "),*SavedGameInstance->PlayerLocation.ToString(),SavedGameInstance->ItemsCollected);
	}
	else
	{
		UE_LOG(SaveGameLogs,Display,TEXT("No saved game exists."));
	}
	txtScore->SetText(FText::FormatOrdered(FText::FromString("Score : {score}/{items}"),score,totalItems));
}

void ATreasureHunterInCppGameModeBase::LoadDLC()
{
	FString DLCPath = FPaths::ConvertRelativePathToFull(FString("../../../TreasureHunterInCpp/Content/Paks/MyDLCTreasureHunterInCpp-Windows.pak"));	
	//Check if Pak File is located in the main paks folder
	if(FPaths::FileExists(DLCPath))
	{
		FPakPlatformFile* PakHandler =new FPakPlatformFile;
		if (!PakHandler->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT("")))
		{
			UE_LOG(LogTemp, Error, TEXT("FPakPlatformFile failed to initialize"));
			return;
		}
	
		FPlatformFileManager::Get().SetPlatformFile(*PakHandler);
		FString NewMountPoint= "/Game";
		//Mount Pak file in the game directory (NOTE :: For Asset Registry the Pak file will be located in /MyDLC folder)
		if(PakHandler->Mount(*DLCPath,0,*NewMountPoint))
		{
			GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red,TEXT("Mount Successful"));
			TArray<FString> PakFiles;
			FString MountedDLCPath = "/Game/Content";
			//Check if file manager can find contents of pak file in the mounted directory
			IFileManager::Get().FindFiles(PakFiles,*MountedDLCPath);
			if(PakFiles.Num()==0)
			{
				UE_LOG(GameModeLogs,Display,TEXT("No Pak Files located in ../../../Content"))
			}
			else
			{
				for(const FString& Files : PakFiles)
				{
					UE_LOG(GameModeLogs,Display,TEXT("Pak Files in location are : %s"),*Files);
				}
			}

			//Get Asset Registry and Scan the plugin foler path (/MyDLC)
			FAssetRegistryModule& RegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			RegistryModule.Get().ScanPathsSynchronous({"/MyDLC"},true);
			TArray<FAssetData> AssetDataList;
			FARFilter DLCFilter;
			DLCFilter.PackagePaths.Add("/MyDLC");
			RegistryModule.Get().GetAssets(DLCFilter,AssetDataList);
			for(FAssetData AssetData : AssetDataList)
			{
				UE_LOG(GameModeLogs,Display,TEXT("Getting Filtered Assets : %s"),*AssetData.GetFullName());
			}
			
			UStaticMesh* DLCMesh = Cast<UStaticMesh>(AssetDataList[2].GetAsset());
			if(DLCMesh)
			{
				UE_LOG(GameModeLogs,Display,TEXT("DLC Mesh Loaded"));
				for(AMyTreasureItem* Item : ItemsInLevel)
				{
					Item->SetTreasureMesh(DLCMesh);
				}
			}
			else
			{
				UE_LOG(GameModeLogs,Warning,TEXT("Failed to Load DLC Mesh"));
			}

			
		}
		else
		{
			txtGameEnd->SetText(FText::FromString("Pak File Doesnt Exist!!!"));
		}
	}
}