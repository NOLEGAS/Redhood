#include "PlayerGameSettings.h"

UPlayerGameSettings* UPlayerGameSettings::Instance = nullptr;

UPlayerGameSettings::UPlayerGameSettings() {

	MasterVolume = 1.f;
	MusicVolume = 1.f;
	SFXVolume = 1.f;
	Brightness = 1.f;
	ShowAudioIndicator = false;
}

UPlayerGameSettings* UPlayerGameSettings::Get() {

	if (!Instance) {
		Instance = NewObject<UPlayerGameSettings>();
		Instance->AddToRoot();
		Instance->LoadPlayerGameSettings();
	}

	return Instance;
}

void UPlayerGameSettings::SavePlayerGameSettings() {

	FString SaveData = FString::Printf(TEXT("MasterVolume=%f\nMusicVolume=%f\nSFXVolume=%f\nBrightness=%f\nShowAudioIndicator=%d"), MasterVolume, MusicVolume, SFXVolume, Brightness, ShowAudioIndicator);
	FString SavePath = FPaths::ProjectSavedDir() / TEXT("PlayerSettings.ini");
	FFileHelper::SaveStringToFile(SaveData, *SavePath);
}

void UPlayerGameSettings::LoadPlayerGameSettings() {

	FString LoadPath = FPaths::ProjectSavedDir() / TEXT("PlayerSettings.ini");
	FString LoadData;
	if (FFileHelper::LoadFileToString(LoadData, *LoadPath)) {
		TArray<FString> Lines;
		LoadData.ParseIntoArrayLines(Lines);

		for (const FString& Line : Lines) {
			FString Key;
			FString Value;
			if (Line.Split(TEXT("="), &Key, &Value)) {
				if (Key == "MasterVolume") {
					MasterVolume = FCString::Atof(*Value);
				}
				else if (Key == "MusicVolume") {
					MusicVolume = FCString::Atof(*Value);
				}
				else if (Key == "SFXVolume") {
					SFXVolume = FCString::Atof(*Value);
				}
				else if (Key == "Brightness")
				{
					Brightness = FCString::Atof(*Value);
				}
				else if (Key == "ShowAudioIndicator") {
					ShowAudioIndicator = FCString::ToBool(*Value);
				}
			}
		}
	}
}

float UPlayerGameSettings::GetAudioVolume(EAudioType AudioType) const {

	switch (AudioType) {
	case EAudioType::Master:
		return MasterVolume;
	case EAudioType::Music:
		return MusicVolume;
	case EAudioType::SFX:
		return SFXVolume;
	default:
		return 1.f;
	}
}

void UPlayerGameSettings::SetAudioVolume(EAudioType AudioType, float NewVolume) {

	switch (AudioType) {
	case EAudioType::Master:
		MasterVolume = NewVolume;
		break;
	case EAudioType::Music:
		MusicVolume = NewVolume;
		break;
	case EAudioType::SFX:
		SFXVolume = NewVolume;
		break;
	default:
		break;
	}
}

float UPlayerGameSettings::GetBrightness() const
{
	return Brightness;
}

void UPlayerGameSettings::SetBrightness(float newBrightness)
{
	Brightness = newBrightness;
}

bool UPlayerGameSettings::GetShowAudioIndicator() const
{
	return ShowAudioIndicator;
}

void UPlayerGameSettings::SetShowAudioIndicator(bool NewValue)
{
	ShowAudioIndicator = NewValue;
}