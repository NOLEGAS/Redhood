// Kent's script

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	Instance,
	Continuous
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_TEAM03_API UInteractableComponent : public UActorComponent, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractType InteractType = EInteractType::Instance;
	
};
