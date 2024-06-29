#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSpline.generated.h"

class USplineComponent;
class UBoxComponent;

UCLASS()
class GP3_TEAM03_API ACameraSpline : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ACameraSpline();

	USplineComponent* GetSpline() {
		return spline;
	}

	bool TargetIsInside() {
		return isInside;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	bool lookAtPlayer = false;

	UPROPERTY(EditAnywhere)
	FVector dirVector;

	UPROPERTY(EditAnywhere)
	bool inverseLookDir = false;

	UPROPERTY(EditAnywhere)
	bool smoothTransition = false;

	UPROPERTY(EditAnywhere)
	bool keepMovementDirection = false;

	UPROPERTY(EditAnywhere)
	float cameraMovementSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float cameraRotationSpeed = 30.0f;

	UPROPERTY(EditAnywhere)
	bool priority = false;

protected:
	UPROPERTY(BlueprintReadWrite)
	bool isInside = false;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* spline = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent*    boxCollider = nullptr;
};
