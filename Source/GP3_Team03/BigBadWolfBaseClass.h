// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyClass.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "BigBadWolfBaseClass.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWolfAttack);
/**
 * 
 */
UCLASS()
class GP3_TEAM03_API ABigBadWolfBaseClass : public ABaseEnemyClass
{
	GENERATED_BODY()
	ABigBadWolfBaseClass();

public:

	UPROPERTY(EditDefaultsOnly)
	float ScareTimer = 2;

	float TimeUntilScared = ScareTimer;

	UFUNCTION(BlueprintCallable)
	void ScareWolf(float deltaTime);

	float StandardSpeed;

	UPROPERTY(EditAnywhere)
	float ScaredSpeed = 100.f;

	virtual void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	bool bDeSpawnOffScreen;

	UFUNCTION(BlueprintCallable)
	void BeginScareTimerReset();
	
	UPROPERTY(EditDefaultsOnly)
	float ResetCoolDown = 3;
	
	virtual void ScriptedMoveToLocation(FVector NewLocation) override;
	
	void ResetScareTimer();

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> RetreatLocations;
	
	FTimerHandle Handle;

	UPROPERTY(EditInstanceOnly)
	bool DespawnWolf = true;

	UPROPERTY(EditAnywhere)
	float PatrolResumeTime = 20.f;
	
	FTimerHandle PatrolResumeHandle;
	
	void ResetFromScare();

	UPROPERTY(BlueprintReadWrite)
	bool ReadyToAttack = false;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWolfAttack WolfAttack;
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	USoundBase* MetaSoundSource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	USoundAttenuation* AttenuationSettings;
};
