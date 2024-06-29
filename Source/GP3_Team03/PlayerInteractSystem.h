// Kent's script

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractSystem.generated.h"

UENUM(BlueprintType)
enum class EInteractState : uint8
{
	Idle  			UMETA(DisplayName = "Idle"),
	Interacting 	UMETA(DisplayName = "Interacting")
};

enum class EInteractType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM03_API UPlayerInteractSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractSystem();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Interact();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EInteractState InteractState = EInteractState::Idle;

	// add event for when the player is focusing on an interactable
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFocused, UInteractableComponent*, Interactable);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableUnfocused);
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractableFocused OnInteractableFocused;

	UPROPERTY(BlueprintAssignable)
	FOnInteractableUnfocused OnInteractableUnfocused;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void CheckForInteractable();
	
	UPROPERTY(VisibleAnywhere)
	UInteractableComponent* CurrentInteractable = nullptr;


	// trace parameters
	// UPROPERTY(EditAnywhere)
	UPROPERTY(EditAnywhere)
	FVector TracePosOffset = FVector::ForwardVector * 100.0f;

	// box trace parameters
	UPROPERTY(EditAnywhere)
	FVector BoxTraceExtent = FVector(50.0f, 50.0f, 50.0f);
	
	FCollisionShape TraceShape = FCollisionShape::MakeBox(FVector(50.0f, 50.0f, 50.0f));

	UPROPERTY(EditAnywhere)
	bool Debug = false;
};
