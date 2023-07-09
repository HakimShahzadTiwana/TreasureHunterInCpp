// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTreasureItem.h"
#include "MyCharacter.h"
#include "TreasureHunterInCppGameModeBase.h"
#include "Blueprint/UserWidget.h"




AMyTreasureItem::AMyTreasureItem()
{
	PrimaryActorTick.bCanEverTick = true;

	//Init Components and attach
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(RootComponent);
	TreasureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Treasure Mesh"));
	TreasureMesh->SetupAttachment(BoxCollider);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this,&AMyTreasureItem::OnBoxOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this,&AMyTreasureItem::OnBoxOverlapEnd);
	ItemId=0;
}
void AMyTreasureItem::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green,TEXT("Spawned"));
	//Get GameMode to access game rules
	TreasureHuntGameMode = Cast<ATreasureHunterInCppGameModeBase>(GetWorld()->GetAuthGameMode());
	//Create widget so that u can add/remove from viewport later
	CreateWidgetPrompt();
	
	

}
void AMyTreasureItem::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if to see its the character
	AMyCharacter* APlayerOther;
	 if(CheckPlayerCharacter(OtherActor,APlayerOther))
	 {
	 	//Display pickup prompt
	 	AddWidgetToViewport();
	 	SetIsInCollider(true);
	 	 if(!bPickupBinded)
	 	 {
	 	 	Bind_E_ToPickup(APlayerOther);
	 	 }
	 }
}

void AMyTreasureItem::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Remove widget and set IsInCollider to false to prevent pickup functionality
	if(Cast<AMyCharacter>(OtherActor))
	{
		if(IsValid(widget))
		{
			RemoveWidgetPrompt();
			SetIsInCollider(false);
		}
	}
}



bool AMyTreasureItem::CheckPlayerCharacter(AActor* OtherActor,AMyCharacter*& MyPlayerCharacter) const
{
	if( AMyCharacter* APlayerOther = Cast<AMyCharacter>(OtherActor))
	{
		MyPlayerCharacter = APlayerOther;
		return true;
	}
	return false;
}



void AMyTreasureItem::CreateWidgetPrompt()
{
	widget= CreateWidget<UUserWidget>(GetGameInstance(),WidgetClass);
}

void AMyTreasureItem::AddWidgetToViewport() const
{
	if(IsValid(widget))
	{
		widget->AddToViewport();
	}
}

void AMyTreasureItem::SetTreasureMesh(UStaticMesh* NewMesh)
{
	TreasureMesh->SetStaticMesh(NewMesh);
	TreasureMesh->SetRelativeScale3D(FVector(0.05f,0.05f,0.05f));
	
}

void AMyTreasureItem::Bind_E_ToPickup( AMyCharacter* APlayerOther)
{
	APlayerOther->InputComponent->BindAction("Pickup",IE_Pressed,this,&AMyTreasureItem::Pickup);
	bPickupBinded=true;
}

void AMyTreasureItem::RemoveWidgetPrompt() const
{
	widget->RemoveFromParent();
}

void AMyTreasureItem::SetIsInCollider(bool IsInCollider)
{
	bIsInCollider=IsInCollider;
}

void AMyTreasureItem::Pickup()
{
	 if(bIsInCollider)
	 {
		 TreasureHuntGameMode->Delegate_UpdateScore.ExecuteIfBound(this);
	 	UE_LOG(GameModeLogs,Display,TEXT("Pick up was called for item with id : %d "),ItemId);
	 }
		this->Destroy();	
	}
	


