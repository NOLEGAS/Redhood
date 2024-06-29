#include "SoundListener.h"
#include "SceneManagement.h"
#include "Components/SphereComponent.h"
#include "SoundPlacement.h"

#define ECC_SoundPlacement ECollisionChannel::ECC_GameTraceChannel2

// Sets default values for this component's properties
USoundListener::USoundListener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...

	//sphere = NewObject<USphereComponent>(this);
}


// Called when the game starts
void USoundListener::BeginPlay()
{
	Super::BeginPlay();
	m_sphere.SetSphere(hearRange);
	m_soundDirection = { 0.0f, 0.0f, 0.0f };
	m_soundLocation  = { 0.0f, 0.0f, 0.0f };
}


// Called every frame
void USoundListener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentDuration -= detectDuration <= 0.0f ? 0.0f : DeltaTime;
	
	if (drawDebugSphere) {
		FColor color = { 255, 0, 0, 255 };
		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), hearRange, 18, color);
	    GEngine->ClearOnScreenDebugMessages();
	}

	FCollisionQueryParams params;
	params.AddIgnoredActor(this->GetOwner());

	FHitResult result;
	if (GetWorld()->SweepSingleByChannel(result, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(),
		FQuat::Identity, ECC_SoundPlacement, m_sphere, params)) {
		
		if (drawDebugSphere && GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Sound Placement Hit")));
		}

		m_soundLocation  = result.GetActor()->GetActorLocation();
		m_soundDirection = GetOwner()->GetActorLocation() - m_soundLocation;
		m_soundDirection.Normalize();
		m_soundDistance = FVector::Distance(GetOwner()->GetActorLocation(), m_soundLocation);
		ASoundPlacement* placement = Cast<ASoundPlacement, AActor>(result.GetActor());
		m_soundType = placement->GetSoundType();

		currentDuration = detectDuration;
	}
}

