// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerCheckpoint.generated.h"

UCLASS()
class GP3_TEAM03_API APlayerCheckpoint : public APlayerStart
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerCheckpoint(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnterCheckpoint, APlayerCheckpoint*, PlayerCheckpoint);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerEnterCheckpoint OnPlayerEnterCheckpoint;

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TriggerSphere = nullptr;
};
