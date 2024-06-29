// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialPopUp.h"


// Sets default values
ATutorialPopUp::ATutorialPopUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	TriggerSphere = CreateDefaultSubobject<USphereComponent>("TriggerRadius");
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetSphereRadius(100.0f);
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ATutorialPopUp::HandleBeginOverlap);
	
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATutorialPopUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutorialPopUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ATutorialPopUp::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if the overlapped actor is the player, then display the tutorial text
	if (OtherActor->ActorHasTag("Player"))
	{
		// display the tutorial text
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TutorialText);
		}

		// broadcast the event
		OnTutorialPopUpTriggered(TutorialText);	
		// destroy self
		Destroy();
	}

}
