
#include "PlayerInteractSystem.h"
#include "InteractableInterface.h"

// Sets default values for this component's properties
UPlayerInteractSystem::UPlayerInteractSystem()
{
	// enable ticking for this component
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerInteractSystem::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerInteractSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForInteractable();
}


void UPlayerInteractSystem::Interact()
{
	// if there is no interactable, return
	if (!CurrentInteractable)
	{
		return;
	}

	// if it's interacting, then call the interact function of the interactable
	if (InteractState == EInteractState::Interacting)
	{
		IInteractableInterface::Execute_Interact(CurrentInteractable->GetOwner(), GetOwner(), this);
		return;
	}
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Interacting with: ") + CurrentInteractable->GetOwner()->GetName());
	
	// call the interact function of the interactable
	IInteractableInterface::Execute_Interact(CurrentInteractable->GetOwner(), GetOwner(), this);


	if (CurrentInteractable->InteractType == EInteractType::Instance)
	{
		InteractState = EInteractState::Idle;
		return;
	}
	InteractState = EInteractState::Interacting;

	OnInteractableUnfocused.Broadcast();
}



void UPlayerInteractSystem::CheckForInteractable()
{
	// if actor is interacting with something, return
	if (InteractState == EInteractState::Interacting)
	{
		return;
	}
	
	TraceShape = FCollisionShape::MakeBox(BoxTraceExtent);

	FMatrix OwnerMatrix = GetOwner()->GetTransform().ToMatrixWithScale();
	FVector WorldTracePosOffset = OwnerMatrix.TransformVector(TracePosOffset);

	if (Debug)
		DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation() + WorldTracePosOffset, BoxTraceExtent, OwnerMatrix.ToQuat(), FColor::Red, false, 0.1f);
	
	TArray<FHitResult> HitResults;
	// have shape rotation be the same as the owner's rotation
	GetWorld()->SweepMultiByChannel(
		HitResults,
		GetOwner()->GetActorLocation() + WorldTracePosOffset,
		GetOwner()->GetActorLocation() + WorldTracePosOffset,
		OwnerMatrix.ToQuat(),
		ECC_Visibility,
		TraceShape);

	// Gets the closest interactable ========================================
	UInteractableComponent* ClosestInteractable = nullptr;
	float ClosestDistance = FLT_MAX;
	for (FHitResult HitResult : HitResults)
	{
		UInteractableComponent* Interactable = HitResult.GetActor()->GetComponentByClass<UInteractableComponent>();
		if (Interactable)
		{
			float Distance = FVector::Dist(GetOwner()->GetActorLocation(), HitResult.GetActor()->GetActorLocation());
			if (!ClosestInteractable || Distance < ClosestDistance)
			{
				ClosestInteractable = Interactable;
				ClosestDistance = Distance;
			}
		}
	}
	// if the closest interactable is different from the current interactable, update the current interactable
	if (ClosestInteractable != CurrentInteractable)
	{
		CurrentInteractable = ClosestInteractable;
		if (CurrentInteractable)
		{
			OnInteractableFocused.Broadcast(CurrentInteractable);
		}
		else
		{
			OnInteractableUnfocused.Broadcast();
		}
	}
}

/* Obsolete code but still good for future references
void UPlayerInteractSystem::InitializeInteractCollider()
{
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &UPlayerInteractSystem::OnInteractOverlapBegin);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &UPlayerInteractSystem::OnInteractOverlapEnd);
}

void UPlayerInteractSystem::OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, 
													AActor* OtherActor, 
													UPrimitiveComponent* OtherComp, 
													int32 OtherBodyIndex, 
													bool bFromSweep, 
													const FHitResult& SweepResult)
{
	// if the new overlapped actor is further away, then ignore it
	if(CurrentInteractable)
	{
		if(FVector::Dist(GetOwner()->GetActorLocation(), OtherActor->GetActorLocation()) > FVector::Dist(GetOwner()->GetActorLocation(), CurrentInteractable->GetActorLocation()))
		{
			return;
		}
	}
	
	// if the actor we are overlapping with is not an interactable, return
	if(!OtherActor->Implements<UInteractableInterface>())
	{
		return;
	}

	// printing message
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Start interacting with: ") + OtherActor->GetName());

	CurrentInteractable = OtherActor;
}

void UPlayerInteractSystem::OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
												  AActor* OtherActor, 
												  UPrimitiveComponent* OtherComp, 
												  int32 OtherBodyIndex)
{
	if(OtherActor == CurrentInteractable)
	{
		CurrentInteractable = nullptr;
	}
}
*/