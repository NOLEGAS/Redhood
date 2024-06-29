#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoundListener.generated.h"

UENUM(BlueprintType)
enum SoundListenerEnums : uint8 {
	DEFAULT = 0 UMETA(DisplayName = "Default"),
	WolfPursuit = 1 UMETA(DisplayName = "Wolf Pursuit"),
	BabaYagaPursuit = 2 UMETA(DisplayName = "BabaYaga Pursuit"),
	BabaYagaBrewing = 3 UMETA(DisplayName = "BabaYaga Brewing")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM03_API USoundListener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoundListener();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	FVector GetRecentSoundLocation() {
		return m_soundLocation;
	}
	UFUNCTION(BlueprintCallable)
	FVector GetRecentSoundDirection() {
		return m_soundDirection;
	}
	UFUNCTION(BlueprintCallable)
	float GetCurrentDetectDuration() {
		return currentDuration;
	}
	UFUNCTION(BlueprintCallable)
	float GetSoundDistance() {
		return m_soundDistance;
	}
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<SoundListenerEnums> GetLastSound() {
		return m_soundType;
	}
protected:
	float currentDuration = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float detectDuration = 2.5f;
		
	UPROPERTY(EditAnywhere)
	float hearRange;

	UPROPERTY(EditAnywhere)
	bool drawDebugSphere = false;

	bool soundInRange;

	FVector m_soundLocation;
	FVector m_soundDirection; 
	float   m_soundDistance = 0.0f;
	TEnumAsByte<SoundListenerEnums> m_soundType;

	FCollisionShape m_sphere = FCollisionShape::MakeSphere(1.0f);
};
