// Fill out your copyright notice in the Description page of Project Settings.


#include "BabaYagaBaseClass.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

ABabaYagaBaseClass::ABabaYagaBaseClass()
{
	LOSOrigin = CreateDefaultSubobject<USceneComponent>("LOSPoint");

	LOSOrigin->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;
}

void ABabaYagaBaseClass::BeginPlay()
{
	Super::BeginPlay();

	if (MetaSoundSource) {

		AudioComponent->SetSound(MetaSoundSource);
	}

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PatrolStationBounds.Num() != PatrolPoints.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			"Baba yaga does not have appropriate workstation bounds, She will not see the player");
		WorkstationBoundsSet = false;
	}

	if (PatrolStationBounds.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			"Baba yaga does not have appropriate workstation bounds, She will not see the player");
		WorkstationBoundsSet = false;
	}
}

void ABabaYagaBaseClass::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "WEG");
	if (WorkstationBoundsSet)
	{
		if (PatrolStationBounds[CurrentPoint]->Illuminated)
		{
			if (PatrolStationBounds[CurrentPoint]->PlayerInBounds && AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
			{
				LOSCheck(DeltaSeconds);
			}
		}
	}
}

void ABabaYagaBaseClass::LOSCheck(float DeltaTime)
{
	FHitResult LOS;

	GetWorld()->LineTraceSingleByChannel(LOS, LOSOrigin->GetComponentLocation(),Player->GetActorLocation(), ECC_Visibility);
	
	if (!LOS.bBlockingHit)
	{
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "!!!");
		DetectingPlayer(DeltaTime);
	}
	else
	{
		DetectionCounter = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "HUH");
	}
}

void ABabaYagaBaseClass::DetectingPlayer(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(DetectionCounter));

	DetectionCounter += DeltaTime;

	if (DetectionThreshold < DetectionCounter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "!!!");
		AudioComponent->Play();
		AttackPlayer();
	}
}


void ABabaYagaBaseClass::AttackPlayer()
{
	//Super::AttackPlayer();

	if (!WorkstationBoundsSet)
	{
		return;
	}
	
	if (CurrentState != EEnemy_State::ScriptedMovement)
	{
		bool MoveLocationFound = false;
		
		AIController->StopMovement();
		
		CurrentState = EEnemy_State::Attacking;

		for (int i = 0; i < PatrolStationBounds.Num(); i++)
		{
			if (PatrolStationBounds[i]->PlayerInBounds)
			{
				AIController->MoveToActor(PatrolPoints[i]);
				MoveLocationFound = true;
				break;
			}
		}

		if (!MoveLocationFound)
		{
			AIController->MoveToLocation(Player->GetActorLocation());
		}
		
	}
	
	DetectionCounter = 0;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Oh no, Baba Yaga is attacking");
}


void ABabaYagaBaseClass::Patrol()
{
	Super::Patrol();

	/*if (AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle && WorkstationBoundsSet)
	{
		FVector TargetDirection = PatrolStationBounds[CurrentPoint]->GetActorLocation()-GetActorLocation();
		TargetDirection.Normalize();

		FVector NewDirection = UKismetMathLibrary::Vector_SlerpVectorToDirection(GetActorForwardVector(), TargetDirection, GetWorld()->DeltaTimeSeconds*2);

		FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(NewDirection);
		
		SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
	}*/
}
