// Fill out your copyright notice in the Description page of Project Settings.


#include "BigBadWolfBaseClass.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


ABigBadWolfBaseClass::ABigBadWolfBaseClass() {

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;
}

void ABigBadWolfBaseClass::BeginPlay()
{
	Super::BeginPlay();

	if (MetaSoundSource) {

		AudioComponent->SetSound(MetaSoundSource);
	}

	StandardSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void ABigBadWolfBaseClass::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bDeSpawnOffScreen)
	{
		if (TimeUntilScared < 0 && AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
		{
			if (DespawnWolf)
			{
				Destroy();
			}
			else
			{
				if (!GetWorld()->GetTimerManager().TimerExists(PatrolResumeHandle))
				{
				GetWorld()->GetTimerManager().SetTimer(PatrolResumeHandle, this,
					&ABigBadWolfBaseClass::ResetFromScare,PatrolResumeTime, false);
				}
			}
		}
	}
}

void ABigBadWolfBaseClass::ScareWolf(float deltaTime)
{
	if (CurrentState == EEnemy_State::ScriptedMovement)
	{
		return;
	}

	if (!GetWorld()->GetTimerManager().TimerExists(Handle))
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
	}

	AttackPlayer();
	
	GetCharacterMovement()->MaxWalkSpeed = ScaredSpeed;

	TimeUntilScared -= deltaTime;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(TimeUntilScared));
	
	if (TimeUntilScared < 0)
	{
		bDeSpawnOffScreen = true;
		if (RetreatLocations.Num() != 0)
		{
			for (AActor* points : RetreatLocations)
			{
				if (points == nullptr)
				{
					Destroy();
				}
			}
			
			AIController->StopMovement();

			AActor* NearestPoint = RetreatLocations[0];
			float DistanceToPoint = (RetreatLocations[0]->GetActorLocation()-GetActorLocation()).Length();
			for (int i = 0; i < RetreatLocations.Num(); i++)
			{
				if ((RetreatLocations[i]->GetActorLocation()-GetActorLocation()).Length() < DistanceToPoint)
				{
					NearestPoint = RetreatLocations[i];
					DistanceToPoint = (RetreatLocations[i]->GetActorLocation()-GetActorLocation()).Length();
				}
			}
			ScriptedMoveToLocation(NearestPoint->GetActorLocation());
		}
		else
		{
			Destroy();
		}
	}
}

void ABigBadWolfBaseClass::BeginScareTimerReset()
{
	GetCharacterMovement()->MaxWalkSpeed = StandardSpeed;
	if (CurrentState != EEnemy_State::ScriptedMovement)
	{
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABigBadWolfBaseClass::ResetScareTimer, ResetCoolDown, false);
	}
	
}


void ABigBadWolfBaseClass::ResetScareTimer()
{
	if (CurrentState != EEnemy_State::ScriptedMovement)
	{
		TimeUntilScared = 2;
	}
}

void ABigBadWolfBaseClass::ScriptedMoveToLocation(FVector NewLocation)
{
	GetCharacterMovement()->MaxWalkSpeed = StandardSpeed;
	
	Super::ScriptedMoveToLocation(NewLocation);
}

void ABigBadWolfBaseClass::ResetFromScare()
{
	CurrentState = EEnemy_State::Patrolling;

	bDeSpawnOffScreen = false;
	
	GetNewPatrolPoint();

	
	float DistanceToPoint = (RetreatLocations[0]->GetActorLocation()-GetActorLocation()).Length();
	
	for (int i = 0; i < PatrolPoints.Num(); i++)
	{
		if ((PatrolPoints[i]->GetActorLocation()-GetActorLocation()).Length() < DistanceToPoint)
		{
			CurrentPoint = i;
			DistanceToPoint = (PatrolPoints[i]->GetActorLocation()-GetActorLocation()).Length();
		}
	}
}
