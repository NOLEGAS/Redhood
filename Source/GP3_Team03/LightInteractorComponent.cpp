// Fill out your copyright notice in the Description page of Project Settings.


#include "LightInteractorComponent.h"

#include "LightInteractableComponent.h"


// Sets default values for this component's properties
ULightInteractorComponent::ULightInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULightInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TMap <AActor*, bool> LightInteractablesThisFrame;
	if (CanInteract)
	{
		FVector Start = GetOwner()->GetActorLocation();
		FVector End = Start + GetOwner()->GetActorForwardVector() * AttenuationRadius;
		if (Debug)
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 1.0f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);
		if (HitResult.bBlockingHit)
		{
			if (ULightInteractableComponent* LightInteractable = HitResult.GetActor()->FindComponentByClass<ULightInteractableComponent>())
			{
				LightInteractable->SetIsInLight(true);
			}
			LightInteractablesThisFrame.Add(HitResult.GetActor(), true);
		}

		float radius = AttenuationRadius;
		float theta = OuterConeAngle / (NumCircles);
		float phi = 360.0f / NumLineTraces;
		theta = FMath::DegreesToRadians(theta);
		phi = FMath::DegreesToRadians(phi);
		float currentTheta, currentPhi;
		
		for (int i = 0; i < NumCircles; i++)
		{
			currentTheta = theta * (i + 1);
			for (int j = 0; j < NumLineTraces; j++)
			{
				currentPhi = phi * j;
				float x = radius * FMath::Cos(currentTheta);
				float y = radius * FMath::Sin(currentTheta) * FMath::Sin(currentPhi);
				float z = radius * FMath::Sin(currentTheta) * FMath::Cos(currentPhi);

				FVector Direction = FVector(x, y, z);
				// rotate direction based on owner's matrix
				Direction = GetOwner()->GetActorRotation().RotateVector(Direction);
				End = Start + Direction;
				
				// perform line trace
				GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);
				// if hit result is not valid
				if (!HitResult.bBlockingHit)
				{
					if (Debug)
						DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 1.0f);
					continue;
				}

				// if valid hit result,
				// check for light interactable component
				ULightInteractableComponent* LightInteractable = HitResult.GetActor()->FindComponentByClass<ULightInteractableComponent>();
				if (!LightInteractable)
				{
					// debug message on what it hit

					if (Debug)
					{
						if (GEngine)
						{
							FString Message = FString::Printf(TEXT("Hit %s"), *HitResult.GetActor()->GetName());
							GEngine->AddOnScreenDebugMessage(-1, 0.01f	, FColor::Red, Message);
						}
						DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 0.1f, 0, 1.0f);
					}
					continue;
				}

				// if there is a light interactable component
				LightInteractable->SetIsInLight(true);
				if (Debug)
					DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, 0, 1.0f);

				LightInteractablesThisFrame.Add(HitResult.GetActor(), true);
			}
		}
	}

	// check for light interactables that were in light last frame but not this frame
	for (auto& Pair : LightInteractablesLastFrame)
	{
		// if not in light this frame, set is in light to false
		if (!LightInteractablesThisFrame.Contains(Pair.Key))
		{
			ULightInteractableComponent* LightInteractable = Pair.Key->FindComponentByClass<ULightInteractableComponent>();
			if (LightInteractable)
			{
				LightInteractable->SetIsInLight(false);
			}
		}
	}

	// set last frame to this frame
	LightInteractablesLastFrame = LightInteractablesThisFrame;
}

/* old code
	float thetaRad = FMath::DegreesToRadians(theta);
	float phiRad = FMath::DegreesToRadians(phi);
	float x = radius * FMath::Cos(thetaRad);
	float y = radius * FMath::Sin(thetaRad) * FMath::Sin(phiRad);
	float z = radius * FMath::Sin(thetaRad) * FMath::Cos(phiRad);
	End = Start + FVector(x, y, z);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 1.0f);
*/