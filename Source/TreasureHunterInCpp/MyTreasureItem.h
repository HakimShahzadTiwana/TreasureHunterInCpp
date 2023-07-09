// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TreasureHunterInCppGameModeBase.h"
#include "MyTreasureItem.generated.h"

UCLASS()
class TREASUREHUNTERINCPP_API AMyTreasureItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTreasureItem();
	inline static int NextItemId=0;
	UPROPERTY()
	int ItemId;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TreasureMesh;
	UFUNCTION(BlueprintCallable)
	void SetTreasureMesh(UStaticMesh* NewMesh);
protected:
	enum EExecutionEnum;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/*Binds E button to Pickup action*/
	UFUNCTION(BlueprintCallable)
	void Bind_E_ToPickup( AMyCharacter* APlayerOther);
	/*Removes Pickup prompt from screen*/
	UFUNCTION(BlueprintCallable)
	void RemoveWidgetPrompt() const;
	/*Sets bIsInColliders to bool value from input
	 * Input Pins: Bool value to set
	 */
	UFUNCTION(BlueprintCallable)
	void SetIsInCollider(bool IsInCollider);
	UFUNCTION(BlueprintCallable)
	void Pickup();
	/*Checks if Actor in input is A Player Character
	 * Input Pins : Actor to check
	 * Output Pins : Casted AMyCharacter object
	 * Returns true if input is a AMyCharacter, else returns false
	 */
	UFUNCTION(BlueprintCallable)
	bool CheckPlayerCharacter(AActor* OtherActor,AMyCharacter*& MyPlayerCharacter) const;
	/*Create Widget Prompt*/
	UFUNCTION(BlueprintCallable)
	void CreateWidgetPrompt();
	//void AddWidgetToViewport(TEnumAsByte<::AMyTreasureItem::EExecutionEnum>& exec) const;
	/*Adds prompt to view port*/
	UFUNCTION(BlueprintCallable)
	void AddWidgetToViewport() const;
	
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> WidgetClass;
	UPROPERTY()
	UUserWidget* widget;
	UPROPERTY()
	UTextBlock* prompt;
	
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;
	UPROPERTY()
	ATreasureHunterInCppGameModeBase* TreasureHuntGameMode;
	UPROPERTY()
	bool bIsInCollider=false;
	UPROPERTY()
	bool bPickupBinded=false;

	
	
};
