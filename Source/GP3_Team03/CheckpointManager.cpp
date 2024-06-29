// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointManager.h"

ACheckpointManager* ACheckpointManager::Instance = nullptr;

// Sets default values
ACheckpointManager::ACheckpointManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACheckpointManager::InitializeManager()
{
	if (Instance != nullptr && Instance != this)
	{
		Destroy();
		return;
	}

	if (Instance == this)
		return;
	
	Instance = this;
	
	for (APlayerCheckpoint* PlayerCheckpoint : PlayerCheckpoints)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Checkpoint added!"));
		
		PlayerCheckpoint->OnPlayerEnterCheckpoint.AddDynamic(this, &ACheckpointManager::OnPlayerEnterCheckpoint);
	}
}

// Called when the game starts or when spawned
void ACheckpointManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeManager();
}


void ACheckpointManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Instance = nullptr;
}

// Called every frame
void ACheckpointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACheckpointManager* ACheckpointManager::GetInstance(UWorld* World)
{
	return Instance;
}

void ACheckpointManager::OnPlayerEnterCheckpoint(APlayerCheckpoint* PlayerCheckpoint)
{
	// check the checkpoint index
	int Index = PlayerCheckpoints.Find(PlayerCheckpoint);
	if (Index > CurrentCheckpointIndex)
	{
		CurrentCheckpointIndex = Index;
	}

	FString Message = FString::Printf(TEXT("Player entered checkpoint %d"), Index);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
}
