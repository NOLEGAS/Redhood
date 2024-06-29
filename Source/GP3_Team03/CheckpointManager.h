// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCheckpoint.h"
#include "GameFramework/Actor.h"
#include "CheckpointManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDying);

UCLASS()
class GP3_TEAM03_API ACheckpointManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckpointManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void InitializeManager();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static ACheckpointManager* GetInstance(UWorld* World);

	TArray<APlayerCheckpoint*> GetCheckpoints() { return PlayerCheckpoints; }
	
	APlayerCheckpoint* GetCurrentCheckpoint()
	{
		if (PlayerCheckpoints.Num() <= 0)
			return nullptr;
		
		if (CurrentCheckpointIndex != -1)
			return PlayerCheckpoints[CurrentCheckpointIndex];

		CurrentCheckpointIndex = 0;
		return PlayerCheckpoints[0];
	}

	//this event will be used to trigger the UI for player death -Gustav
	UPROPERTY(BlueprintAssignable)
	FPlayerDying PlayerDying;
	
private:
	static ACheckpointManager* Instance;

	UPROPERTY(EditInstanceOnly, Category = "Checkpoints")
	TArray<APlayerCheckpoint*> PlayerCheckpoints;

	UFUNCTION()
	void OnPlayerEnterCheckpoint(APlayerCheckpoint* PlayerCheckpoint);

	UPROPERTY(VisibleAnywhere)
	int CurrentCheckpointIndex = -1;
};
