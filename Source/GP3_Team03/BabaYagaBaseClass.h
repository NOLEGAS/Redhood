// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BabaYagaWorkstationBounds.h"
#include "BaseEnemyClass.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "BabaYagaBaseClass.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM03_API ABabaYagaBaseClass : public ABaseEnemyClass
{
	GENERATED_BODY()
	ABabaYagaBaseClass();

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ABabaYagaWorkstationBounds*> PatrolStationBounds;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* LOSOrigin = nullptr;

	UPROPERTY()
	ACharacter* Player = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void LOSCheck(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void DetectingPlayer(float DeltaTime);
	
	virtual void AttackPlayer() override;

	virtual void Patrol() override;

	UPROPERTY(EditAnywhere)
	float DetectionThreshold = 4.f;

	float DetectionCounter = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool WorkstationBoundsSet = true;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	USoundBase* MetaSoundSource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	USoundAttenuation* AttenuationSettings;
};
