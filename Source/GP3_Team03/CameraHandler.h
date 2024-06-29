#pragma once

#include "Camera/CameraActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraHandler.generated.h"

class UCameraComponent;
class ACameraSpline;

UCLASS()
class GP3_TEAM03_API ACameraHandler : public ACameraActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateCameraMovement(const float dt);

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* actor);

	UFUNCTION(BlueprintCallable)
	void UpdateSplineQueue();

	UFUNCTION(BlueprintCallable)
	void FetchAllSplines();

	UFUNCTION(BlueprintCallable)
	bool HasChangedSpline() {
		return splineChanged;
	}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetMainCamera() {
		return GetCameraComponent();
	}


private:
	void SwitchSpline();
	FVector GetLookDirection(ACameraSpline* spline);

	TArray<ACameraSpline*> cameraSplines;

	AActor* target = nullptr;

	UPROPERTY(EditAnywhere)
	bool startClosestSpline;

	TArray<int> splineQueue;
	int previousSpline;
	int currentSpline;

	bool splineChanged = false;
};
