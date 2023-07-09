// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Misc/ConfigCacheIni.h"
#include "Enemy.h"
#include "MyDeveloperSettings.h"
#include "MyTreasureItem.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(MovementLogs);
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing Components
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	//Make Mesh to face forward
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));
	
	//Attach Components to mesh
	SpringArmComp->SetupAttachment(GetMesh());
	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);
	BoxCollider->SetupAttachment(RootComponent.Get());
	//Configure Camera Settings
	CameraComp->SetWorldLocation(FVector(0.0f,0.0f,0.0f));
	CameraComp->SetFieldOfView(120.0f);
	
	//Configure Spring arm settings
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetOffset.Z = 50.0;

	//Configure Character Movement Settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this,&AMyCharacter::OnBoxOverlap);
	//First Getting Player Speed From Developer Settings, if its 0 there then get from Config folder
	const UMyDeveloperSettings* DevSettings = GetDefault<UMyDeveloperSettings>();
	float PlayerWalkSpeed = DevSettings->PlayerSpeed;
		if(PlayerWalkSpeed==0)
	{
		FString ConfigPath = "C:/Users/hakim/Unreal_Projects/TreasureHunterInCpp/Saved/Config/WindowsEditor/TreasureHuntConfigs.ini";
		GConfig->GetFloat(TEXT("/Script/TreasureHunterInCpp.MyCharacter"),TEXT("ConfigPlayerSpeed"),ConfigPlayerSpeed,*ConfigPath);
		UE_LOG(MovementLogs,Display,TEXT("Setting PlayerSpeed from Config Settings. Value : %f"),ConfigPlayerSpeed);
		GetCharacterMovement()->MaxWalkSpeed = ConfigPlayerSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = PlayerWalkSpeed;
		UE_LOG(MovementLogs,Display,TEXT("Setting PlayerSpeed from Dev Settings. Value : %f"),PlayerWalkSpeed);

	}
	
}

//Shifted Functionality to a custom player controller
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){}

FRotator AMyCharacter::GetDirection() const
{
	//Get rotation from controller and then break it into roll yaw and pitch
	FRotator rotator = GetControlRotation();
	float Rot_Roll, Rot_Yaw, Rot_Pitch;
	UKismetMathLibrary::BreakRotator(rotator,Rot_Roll,Rot_Pitch,Rot_Yaw);
	//Return the yaw part of the rotation
	return UKismetMathLibrary::MakeRotator(0,0,Rot_Yaw);
	
}

void AMyCharacter::MoveForward(float Value)
{
	//Get the forward direction vector and Add movement
	FVector forwardDir = UKismetMathLibrary::GetForwardVector(GetDirection());
	AddMovementInput(forwardDir,Value);	
}

void AMyCharacter::MoveRight(float Value)
{
	//Get the right direction vector and Add movement
	FVector rightDir = UKismetMathLibrary::GetRightVector(GetDirection());
	AddMovementInput(rightDir,Value);
}

void AMyCharacter::SetSprintSpeed()
{
	//if shift pressed , start sprinting, if released go back to normal speed
	if (!bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		bIsSprinting=true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		bIsSprinting=false;
	}
}

void AMyCharacter::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if(CheckEnemy(OtherActor))
	{
		ATreasureHunterInCppGameModeBase *TreasureHuntGameMode = Cast<ATreasureHunterInCppGameModeBase>(GetWorld()->GetAuthGameMode());
		TreasureHuntGameMode->Delegate_GameOver.ExecuteIfBound(nullptr);
	}
	
}

bool AMyCharacter::CheckEnemy(AActor* OtherActor) const
{
	if(Cast<AEnemy>(OtherActor))
	{
		return true;
	}
	return false;
}

void AMyCharacter::CallGameOver() const{}

/*----------------------------------------------------------Commented Out---------------------------------------------//

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		
	Bind keyboard inputs
	 PlayerInputComponent->BindAxis("MoveForward",this,&AMyCharacter::MoveForward);
	 PlayerInputComponent->BindAxis("MoveRight",this,&AMyCharacter::MoveRight);
	
	 //Bind mouse inputs
	 PlayerInputComponent->BindAxis("LookUp",this,&AMyCharacter::AddControllerPitchInput);
	 PlayerInputComponent->BindAxis("Turn",this,&AMyCharacter::AddControllerYawInput);
	
	 //Bind Actions
	 //Sprint
	 PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AMyCharacter::SetSprintSpeed);
	 PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AMyCharacter::SetSprintSpeed);
	 //Jump
	 PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMyCharacter::Jump);
	 PlayerInputComponent->BindAction("Jump",IE_Released,this,&AMyCharacter::StopJumping);
	
	
}

 void AMyCharacter::CallGameOver() const
 {
 	const ATreasureHunterInCppGameModeBase *TreasureHuntGameMode = Cast<ATreasureHunterInCppGameModeBase>(GetWorld()->GetAuthGameMode());
 	TreasureHuntGameMode->GameOver();
 }




*/














