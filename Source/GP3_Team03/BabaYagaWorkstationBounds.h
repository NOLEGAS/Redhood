// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BabaYagaWorkstationBounds.generated.h"

UCLASS()
class GP3_TEAM03_API ABabaYagaWorkstationBounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABabaYagaWorkstationBounds();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Bounds = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool PlayerInBounds = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Illuminated = false;

	UFUNCTION()
	void HandleEnterBounds(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	UFUNCTION()
	void HandleLeaveBounds(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex);
};
