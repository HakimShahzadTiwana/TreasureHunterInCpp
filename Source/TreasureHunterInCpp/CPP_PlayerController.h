// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TREASUREHUNTERINCPP_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACPP_PlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	AMyCharacter* PlayerChar;
	/*Calls Forward Movement Implementation of the pawn controller is connected with
	*Input Pins: Axis Value
	 */
	UFUNCTION(BlueprintCallable,Category="CPP_PlayerController")
	void CallMoveForward(float Val) const;
	/*Calls Right Movement Implementation of the pawn controller is connected with
	*Input Pins: Axis Value
	*/
	UFUNCTION(BlueprintCallable,Category="CPP_PlayerController")
	void CallMoveRight(float Val) const;
	/*Calls Sprint Implementation of the pawn controller is connected with
	*Input Pins: Axis Value
	*/
	UFUNCTION(BlueprintCallable,Category="CPP_PlayerController")
	void CallSprint() const;
	/*Calls Jump Implementation of the pawn controller is connected with
	*Input Pins: Axis Value
	*/
	UFUNCTION(BlueprintCallable,Category="CPP_PlayerController")
	void CallJump() const;
	/*Calls Jump End Implementation of the pawn controller is connected with
	*Input Pins: Axis Value
	*/
	UFUNCTION(BlueprintCallable,Category="CPP_PlayerController")
	void CallJumpEnd() const;
	/*Calls Turn Implementation of the pawn controller is connected with
	*Input Pins: Axis Value
	*/
	UFUNCTION(BlueprintCallable,Category="CPP_PlayerController")
	void CallTurn(float Val) const;
	/*Calls LookUp Implementation of the pawn controller is connected with
	*Input Pins: Axis Value
	*/
	UFUNCTION(BlueprintCallable,Category="CPP_PlayerController")
	void CallLookUp(float Val) const;
};
