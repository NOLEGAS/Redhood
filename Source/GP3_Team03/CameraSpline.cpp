#include "CameraSpline.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"

ACameraSpline::ACameraSpline()
{
	PrimaryActorTick.bCanEverTick = true;
	boxCollider   = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	spline		  = CreateDefaultSubobject<USplineComponent>(TEXT("Camera Spline"));
	spline->AttachToComponent(boxCollider, FAttachmentTransformRules::KeepRelativeTransform);

	dirVector = { 0.0f, 1.0f, 0.0f };
}
void ACameraSpline::BeginPlay()
{
	Super::BeginPlay();	
}
void ACameraSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

