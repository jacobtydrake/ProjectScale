
#include "PSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectScale/PSHUD.h"
#include "Kismet/GameplayStatics.h"

void APSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerCharacterMappingContext, 0);
	}

	if (TObjectPtr<APSHUD> LoadedHUD = Cast<APSHUD>(GetHUD()))
	{
		CachedHUD = LoadedHUD;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get PSHUD"));
	}
}

void APSPlayerController::OnPauseButtonPressed()
{
	if (CachedHUD)
	{
		CachedHUD->TogglePauseScreen();
	}

	if (IsPaused())
	{
		SetPause(false);
		FInputModeGameOnly InputModeData;
		SetInputMode(InputModeData);

		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CaptureDuringMouseDown);
		bShowMouseCursor = false;
	}
	else
	{
		SetPause(true);
		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputModeData);

		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::NoCapture);
		bShowMouseCursor = true;
	}

}

void APSPlayerController::OnCharacterDeath()
{
	// Pause the game
	/*SetPause(true);*/

	// Change input mode to UI only
	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);

	bShowMouseCursor = true;

	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");


	//// Display a death screen or transition to a death state
	//if (CachedHUD)
	//{
	//	CachedHUD->ShowDeathScreen(); // Example function call to show death screen
	//}

	//// Optionally, set a timer to restart the level or go back to the main menu
	//FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this, &APSPlayerController::RestartLevel, RestartDelay, false);

	UE_LOG(LogTemp, Display, TEXT("void APSPlayerController::OnCharacterDeath()"));
}
