// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(MovementLogs, Display, All);
UCLASS(Config=TreasureHuntConfigs)
class TREASUREHUNTERINCPP_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//CONSTRUCTOR
	AMyCharacter();
	UFUNCTION(BlueprintCallable,Category="CPP_Character")
	void MoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category="CPP_Character")
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable,Category="CPP_Character")
	void SetSprintSpeed();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/*Calls Check Gameover function from TreasureHunterGameModeBase*/
	UFUNCTION(BlueprintCallable,Category="CPP_Character")
	void CallGameOver() const;

	
	/*Checks if the other Actor is an enemy.
	 * Input Pins: OtherActor->Reference of an Actor
	 * Returns true if Actor is an "Enemy"
	 */
	UFUNCTION(BlueprintCallable,Category="CPP_Character")
	bool CheckEnemy(AActor* OtherActor) const;
	
	/*Checking if this shows up in blueprint*/
	UFUNCTION(BlueprintCallable,Category="CPP_Character")
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable,Category="CPP_Character")
	FRotator GetDirection() const;

	//Spring Arm Component to follow the camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	//Player follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp; 

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;

	UPROPERTY(Config)
	float ConfigPlayerSpeed;
	
	UPROPERTY()
	bool bIsSprinting=false;

	
public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
