
#include "PSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectScale/PSHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PSScoreController.h"
#include "ProjectScale/Actors/PSPickupItem.h"
#include "ProjectScale/Actors/PSCircularEnemySpawner.h"
#include "ProjectScale/Actors/PSEnemy.h"
#include "Camera/CameraActor.h"
#include "ProjectScale/Actors/PSCharacter.h"

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

	PSScoreController = NewObject<UPSScoreController>(this);
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
	// Change input mode to UI only
	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);

	bShowMouseCursor = true;

	//SetPause(true);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APSCircularEnemySpawner::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		APSCircularEnemySpawner* Spawner = Cast<APSCircularEnemySpawner>(Actor);
		if (Spawner)
		{
			Spawner->StopSpawningEnemies();
		}
	}

	if (SelectedCameraClass)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SelectedCameraClass, FoundActors);
		if (FoundActors.Num() > 0)
		{
			AActor* CameraActor = FoundActors[0];
			SetViewTargetWithBlend(CameraActor, 3.0f);
		}
	}

	if (PSScoreController && CachedHUD && PSScoreController)
	{
		PSScoreController->InitializeCachedHUD(CachedHUD);
		int32 TotalScore = PSScoreController->GetTotalScore();
		TMap<EPickupItemType, int32> ItemCounts = PSScoreController->GetItemPickupCounts();
		CachedHUD->DisplayScoreScreen(ItemCounts, TotalScore);
	}
}

void APSPlayerController::StartScoreScreenProcess()
{
	Cleanup();

	if (PSScoreController)
	{
		PSScoreController->StartSpawningScales(FVector(0.0f, 0.0f, 750.0f), 3.0f, 0.1f, FVector(200.0f, 200.0f, 0.0f));
	}
}

void APSPlayerController::Cleanup() // really shouldn't be in player controller, but oh well
{
	TArray<AActor*> Actors;
	// destroy all enemies
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APSEnemy::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		Actor->Destroy();
	}
	Actors.Empty();

	// destroy all pickup items
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APSPickupItem::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		Actor->Destroy();
	}
	Actors.Empty();

	if (AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), APSCharacter::StaticClass()))
	{
		//if (APSCharacter* MyPSCharacter = Cast<APSCharacter>(Actor))
		//{
		//	MyPSCharacter->SetActorLocation(FVector)
		//}

		Actor->SetActorHiddenInGame(true);
	}
}
