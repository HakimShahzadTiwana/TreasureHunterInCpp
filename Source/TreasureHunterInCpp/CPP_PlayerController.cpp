// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerController.h"
#include "GameFramework/PlayerController.h"

ACPP_PlayerController::ACPP_PlayerController()
{
	
}

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerChar = Cast<AMyCharacter>(GetCharacter());
	
}

void ACPP_PlayerController::SetupInputComponent()
{ 
	Super::SetupInputComponent();
	//TODO wth is going on here
	InputComponent->BindAxis("MoveForward",this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void(float)>::RetValType(float)>::Type(&ACPP_PlayerController::CallMoveForward));
	InputComponent->BindAxis("MoveRight",this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void(float)>::RetValType(float)>::Type(&ACPP_PlayerController::CallMoveRight));
	InputComponent->BindAction("Jump",IE_Pressed,this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void()>::RetValType()>::Type(&ACPP_PlayerController::CallJump));
	InputComponent->BindAction("Jump",IE_Released,this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void()>::RetValType()>::Type(&ACPP_PlayerController::CallJumpEnd));
	InputComponent->BindAction("Sprint",IE_Pressed,this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void()>::RetValType()>::Type(&ACPP_PlayerController::CallSprint));
	InputComponent->BindAction("Sprint",IE_Released,this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void()>::RetValType()>::Type(&ACPP_PlayerController::CallSprint));
	InputComponent->BindAxis("LookUp",this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void(float)>::RetValType(float)>::Type(&ACPP_PlayerController::CallLookUp));
	InputComponent->BindAxis("Turn",this,TMemFunPtrType<false, ACPP_PlayerController, TDelegate<void(float)>::RetValType(float)>::Type(&ACPP_PlayerController::CallTurn));
}



void ACPP_PlayerController::CallMoveForward(const float Val) const

{
	if(IsValid(PlayerChar))
	{
		PlayerChar->MoveForward(Val);
	}
}

void ACPP_PlayerController::CallMoveRight(const float Val) const
{
	if(IsValid(PlayerChar))
	{
		PlayerChar->MoveRight(Val);
	}
}

void ACPP_PlayerController::CallJump() const
{
	if(IsValid(PlayerChar))
	{
		PlayerChar->Jump();
	}
}
 void ACPP_PlayerController::CallJumpEnd() const
 {
	 if(IsValid(PlayerChar))
	 {
		 PlayerChar->StopJumping();
	 }
 }

void ACPP_PlayerController::CallSprint() const
{
	if(IsValid(PlayerChar))
	{
		PlayerChar->SetSprintSpeed();
	}

}

void ACPP_PlayerController::CallTurn(const float Val) const
{
	if(IsValid(PlayerChar))
	{
		PlayerChar->AddControllerYawInput(Val);
	}
}

void ACPP_PlayerController::CallLookUp(const float Val) const
{
	if(IsValid(PlayerChar))
	{
		PlayerChar->AddControllerPitchInput(Val);
	}
}
