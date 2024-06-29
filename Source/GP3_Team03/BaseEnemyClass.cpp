// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyClass.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
ABaseEnemyClass::ABaseEnemyClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEnemyClass::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());

	if (AIController == nullptr)
	{
		Destroy();
	}
}

// Called every frame
void ABaseEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState == EEnemy_State::ScriptedMovement && DespawnUponScriptedMoveCompletion)
	{
		if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			Destroy();
		}
	}
}


void ABaseEnemyClass::ScriptedMoveToLocation(FVector NewLocation)
{
	CurrentState = EEnemy_State::ScriptedMovement;
	
	AIController->StopMovement();
	
	AIController->MoveToLocation(NewLocation);
}

void ABaseEnemyClass::Patrol()
{
	if (CurrentState == EEnemy_State::Patrolling && PatrolPoints.Num() != 0)
	{
		AIController->MoveToLocation(PatrolPoints[CurrentPoint]->GetActorLocation(), 1.f);

		if (AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
		{
			if (!GetWorld()->GetTimerManager().TimerExists(PatrolPointSelectTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(PatrolPointSelectTimerHandle, this, &ABaseEnemyClass::GetNewPatrolPoint, TimeBetweenPatrolPoints, false);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(PatrolPointSelectTimerHandle);
		}
	}
}

void ABaseEnemyClass::GetNewPatrolPoint()
{
	if (PatrolRandomly)
	{
		int newPoint = UKismetMathLibrary::RandomInteger(PatrolPoints.Num());

		if (newPoint == CurrentPoint)
		{
			if (CurrentPoint == PatrolPoints.Num()-1)
			{
				CurrentPoint = 0;
			}
			else
			{
				CurrentPoint++;
			}
		}
		else
		{
			CurrentPoint = newPoint;
		}
	}
	else
	{
		if (CurrentPoint == PatrolPoints.Num()-1)
		{
			CurrentPoint = 0;
		}
		else
		{
			CurrentPoint++;
		}
	}
}

void ABaseEnemyClass::SearchLocation(FVector Location)
{
	if (CurrentState == EEnemy_State::Patrolling)
	{
		CurrentState = EEnemy_State::Searching;

		AIController->MoveToLocation(Location);

		//GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(SearchTime));
		GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, this , &ABaseEnemyClass::ResumePatrol, SearchTime);
	}
}

void ABaseEnemyClass::ResumePatrol()
{
	if (CurrentState != EEnemy_State::ScriptedMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Patrol resumed");
		CurrentState = EEnemy_State::Patrolling;
	}
}


void ABaseEnemyClass::AttackPlayer()
{
	if (CurrentState != EEnemy_State::ScriptedMovement)
	{
		AIController->StopMovement();
		
		CurrentState = EEnemy_State::Attacking;

		GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
		
		AIController->MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	}
}

