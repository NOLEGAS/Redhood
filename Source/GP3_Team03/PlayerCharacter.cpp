#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UPlayerGameSettings* PlayerGameSettings = UPlayerGameSettings::Get();
//	if (PlayerGameSettings) {
//
//		float MasterVolume = PlayerGameSettings->MasterVolume;
//	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

		if (PlayerController) {
			USceneComponent* ListenerComponent = GetRootComponent();

			if (ListenerComponent) {
				PlayerController->SetAudioListenerOverride(ListenerComponent, FVector::ZeroVector, FRotator::ZeroRotator);
			}
		}
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

