// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryGameMode.h"
#include "CheckpointManager.h"
#include "EngineUtils.h"


AActor* AStoryGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// find existing checkpoint manager not through get instance
	// ACheckpointManager* CheckpointManager 
	if (!GetWorld())
		return Super::ChoosePlayerStart_Implementation(Player);

	ACheckpointManager* CheckpointManager = nullptr;
	
	for (TActorIterator<ACheckpointManager> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		CheckpointManager = *ActorIterator;
		break;
	}
	
	CheckpointManager->InitializeManager();

	// if there is no checkpoint manager
	if (CheckpointManager == nullptr)
		return Super::ChoosePlayerStart_Implementation(Player);
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Checkpoint manager found!"));
	}
	
	// get the first checkpoint
	TArray<APlayerCheckpoint*> FirstCheckpoint = CheckpointManager->GetCheckpoints();

	if (FirstCheckpoint.Num() <= 0)
		return Super::ChoosePlayerStart_Implementation(Player);
	
	// return the first checkpoint
	return FirstCheckpoint[0];
}
