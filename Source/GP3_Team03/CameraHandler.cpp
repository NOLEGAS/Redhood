// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraHandler.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CameraSpline.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACameraHandler::ACameraHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	previousSpline = -1;
	currentSpline = -1;

	startClosestSpline = false;
}

// Called when the game starts or when spawned
void ACameraHandler::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraHandler::UpdateCameraMovement(const float dt)
{
	if (currentSpline > -1) {
		ACameraSpline* spline = cameraSplines[currentSpline];
		FVector point = spline->GetSpline()->FindLocationClosestToWorldLocation(target->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector newPosition = UKismetMathLibrary::VInterpTo(GetCameraComponent()->GetComponentLocation(), point, dt, spline->cameraMovementSpeed);
		GetCameraComponent()->SetWorldLocation(newPosition);

		if (spline->lookAtPlayer) {
			FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(GetCameraComponent()->GetComponentLocation(), target->GetActorLocation());
			GetCameraComponent()->SetWorldRotation(lookAt);
		}
		else {
			FVector  dir = GetLookDirection(spline);
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(dir, dir + dir);

			rotation = UKismetMathLibrary::RInterpTo_Constant(GetCameraComponent()->GetComponentRotation(), rotation, dt, spline->cameraRotationSpeed);
			GetCameraComponent()->SetWorldRotation(rotation);
		}
	}
	else {
		if (GEngine) {
			GEngine->ClearOnScreenDebugMessages();
			if(cameraSplines.Num() < 1)
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, FString::Printf(TEXT("NO SPLINES!")));
			if (splineQueue.Num() < 1)
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("SPLINE QUEUE IS EMPTY")));
		}

	}
}

void ACameraHandler::SetTarget(AActor* actor)
{
	target = actor;
}

// Called every frame
void ACameraHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACameraHandler::SwitchSpline()
{
	ACameraSpline* spline = cameraSplines[currentSpline];
	if (!spline->smoothTransition) {
		FVector location = spline->GetSpline()->FindLocationClosestToWorldLocation(target->GetActorLocation(), ESplineCoordinateSpace::World);
		GetCameraComponent()->SetWorldLocation(location);

		FVector dir = GetLookDirection(spline);
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(dir, dir + dir);
		GetCameraComponent()->SetWorldRotation(rotation);
	}
}

// Gets the wanted look direction based on given direction, defaults to Right vector if not specified
FVector ACameraHandler::GetLookDirection(ACameraSpline* spline)
{
	FVector v;

	FRotator rotation = spline->GetSpline()->FindRotationClosestToWorldLocation(target->GetActorLocation(), ESplineCoordinateSpace::World);
	
	if (spline->dirVector.Z >= 1)
		v = UKismetMathLibrary::GetUpVector(rotation);
	else if (spline->dirVector.Y >= 1)
		v = UKismetMathLibrary::GetRightVector(rotation);
	else
		v = UKismetMathLibrary::GetForwardVector(rotation);
	
	if (spline->inverseLookDir)
		v *= FVector(-1.0f);

	return v;
}

// Functions called in blueprint 
void ACameraHandler::UpdateSplineQueue()
{
	splineChanged = false;
	// Check if Target is inside any of our splines
	for (int i = 0; i < cameraSplines.Num(); i++)
		if (cameraSplines[i]->TargetIsInside() && !splineQueue.Contains(i)) {
			// If spline is a priority then insert it to the top, will override other priority splines
			if (cameraSplines[i]->priority) {
				if(cameraSplines[i]->keepMovementDirection)
					splineChanged = true;
				
				splineQueue.Insert(i, 0);	
			}
			else
				splineQueue.Add(i);

			currentSpline = splineQueue[0];
			// set currentSpline to top of the list 

			// if splineQueue is emtpy or priority we do a SwitchSpline
			if (splineQueue.Num() == 1 || cameraSplines[i]->priority)
				SwitchSpline();

			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Spline Added")));
		}

	// Remove splines that the player is not currently inside of, if spline is current Active spline we set previous
	for (int i = 0; i < splineQueue.Num(); i++)
	{
		const int index = splineQueue[i];
		ACameraSpline* spline = cameraSplines[index];
		if (!spline->TargetIsInside()) {
			
			splineQueue.RemoveAt(i);
			if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Spline Removed")));
			
			// only do a switchspline if spline on top of the queue is being removed
			if(i == 0){
				
				previousSpline = index;
				if (splineQueue.Num() > 0) {
					currentSpline = splineQueue[0];

					if (cameraSplines[currentSpline]->keepMovementDirection)
						splineChanged = true;
					
					SwitchSpline();
				}
				else
					currentSpline = previousSpline;
	
			}
		}
	}
}

void ACameraHandler::FetchAllSplines()
{
	// Only run CameraHandler if Target != NULL
	if (target) {
		TArray<AActor*> splines;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraSpline::StaticClass(), splines);

		if (splines.IsEmpty()) {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, { 255, 0, 0 }, FString::Printf(TEXT("No Splines in Scene!")));
		}
		else {
			float minDistance = FLT_MAX;
			for (int i = 0; i < splines.Num(); i++) {
				cameraSplines.Add(Cast<ACameraSpline>(splines[i]));	
			}
		}
	}
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, { 255, 0, 0 }, FString::Printf(TEXT("Player has not been set and is NULL!")));
}


