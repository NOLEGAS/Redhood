// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BaseEnemyClass.generated.h"


UENUM(BlueprintType)
enum class EEnemy_State  : uint8{ Patrolling,  ScriptedMovement, Attacking, Searching};

UCLASS()
class GP3_TEAM03_API ABaseEnemyClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemyClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, meta=(ShortToolTip="Overrides other behaviors"))
	virtual void ScriptedMoveToLocation(FVector NewLocation);

	UFUNCTION(BlueprintCallable)
	virtual void Patrol();

	UPROPERTY(EditAnywhere)
	bool PatrolRandomly = false;

	UPROPERTY(BlueprintReadOnly)
	int CurrentPoint = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<AActor*> PatrolPoints;
	
	UFUNCTION(BlueprintCallable)
	virtual void GetNewPatrolPoint();

	UPROPERTY(BlueprintReadWrite)
	EEnemy_State CurrentState = EEnemy_State::Patrolling;
	
	UPROPERTY()
	AAIController* AIController = nullptr;

	FTimerHandle PatrolPointSelectTimerHandle;

	UPROPERTY(EditAnywhere)
	float TimeBetweenPatrolPoints = 2;

	FTimerHandle SearchTimerHandle;

	UPROPERTY(EditAnywhere)
	float SearchTime = 5.f;

	void ResumePatrol();
	
	UFUNCTION(BlueprintCallable)
	virtual void SearchLocation(FVector Location);
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackPlayer();

	UPROPERTY(BlueprintReadWrite)
	bool DespawnUponScriptedMoveCompletion = false;
};
