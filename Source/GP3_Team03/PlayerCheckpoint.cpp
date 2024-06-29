// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCheckpoint.h"


// Sets default values
APlayerCheckpoint::APlayerCheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	TriggerSphere = CreateDefaultSubobject<USphereComponent>("TriggerRadius");
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetSphereRadius(100.0f);
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCheckpoint::HandleBeginOverlap);
	
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerCheckpoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCheckpoint::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if the overlapped actor is the player, then display the tutorial text
	if (OtherActor->ActorHasTag("Player"))
	{
		// broadcast the event
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player entered checkpoint!"));

		OnPlayerEnterCheckpoint.Broadcast(this);

	}
}
