// Fill out your copyright notice in the Description page of Project Settings.


#include "BabaYagaWorkstationBounds.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABabaYagaWorkstationBounds::ABabaYagaWorkstationBounds()
{
	Bounds = CreateDefaultSubobject<UBoxComponent>("StationBounds");

	Bounds->OnComponentBeginOverlap.AddDynamic(this, &ABabaYagaWorkstationBounds::HandleEnterBounds);
	Bounds->OnComponentEndOverlap.AddDynamic(this, &ABabaYagaWorkstationBounds::HandleLeaveBounds);
	
	Bounds->SetupAttachment(RootComponent);
}

void ABabaYagaWorkstationBounds::HandleEnterBounds(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		PlayerInBounds = true;
	}
}

void ABabaYagaWorkstationBounds::HandleLeaveBounds(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		PlayerInBounds = false;
	}
}
