#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "PlayerGameSettings.generated.h"

UENUM(BlueprintType)
enum class EAudioType : uint8 {

	Master,
	Music,
	SFX
};

UCLASS(Blueprintable)
class GP3_TEAM03_API UPlayerGameSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Settings")
	static UPlayerGameSettings* Get();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SavePlayerGameSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadPlayerGameSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	float GetAudioVolume(EAudioType AudioType) const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetAudioVolume(EAudioType AudioType, float NewVolume);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float SFXVolume;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	float Brightness;

	UFUNCTION(BlueprintCallable, Category = "Visual")
	float GetBrightness() const;

	UFUNCTION(BlueprintCallable, Category = "Visual")
	void SetBrightness(float newBrightness);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool ShowAudioIndicator;

	UFUNCTION(BlueprintCallable, Category = "Visual")
	bool GetShowAudioIndicator() const;

	UFUNCTION(BlueprintCallable, Category = "Visual")
	void SetShowAudioIndicator(bool NewValue);
	
private:

	UPlayerGameSettings();

	static UPlayerGameSettings* Instance;

};