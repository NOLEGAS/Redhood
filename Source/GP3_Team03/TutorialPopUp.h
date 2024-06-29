// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "TutorialPopUp.generated.h"

UCLASS()
class GP3_TEAM03_API ATutorialPopUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATutorialPopUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// event for when the tutorial pop up is triggered
	UFUNCTION(BlueprintImplementableEvent)
	void OnTutorialPopUpTriggered(const FString& TextToDisplay);
	
private:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* TriggerSphere = nullptr;
	
	// text to display
	UPROPERTY(EditAnywhere)
	FString TutorialText = "Tutorial Text";
};
