// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StoryGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM03_API AStoryGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
