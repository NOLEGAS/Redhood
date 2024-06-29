// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightInteractorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_TEAM03_API ULightInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULightInteractorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void SetCanInteract(bool canInteract) { CanInteract = canInteract; }
	
private:
	// attenuation radius
	UPROPERTY(EditAnywhere, Category = "Light Interactor")
	float AttenuationRadius = 1000.0f;
	
	// outer cone angle
	UPROPERTY(EditAnywhere, Category = "Light Interactor")
	float OuterConeAngle = 45.0f;
	
	// number of line traces per circle
	UPROPERTY(EditAnywhere, Category = "Light Interactor")
	int NumLineTraces = 15;
	// number of degrees between outer and center
	UPROPERTY(EditAnywhere, Category = "Light Interactor")
	int NumCircles = 5;

	TMap<AActor*, bool> LightInteractablesLastFrame;

	bool CanInteract = false;

	UPROPERTY(EditAnywhere, Category = "Light Interactor")
	bool Debug = false;
};
