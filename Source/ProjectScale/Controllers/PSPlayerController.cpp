// Written by Jacob Drake - 2023

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
#include "Components/ActorComponent.h"

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

	// cache PSCharacter
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APSCharacter::StaticClass(), FoundCharacters);
	if (FoundCharacters.Num() > 0)
	{
		CachedCharacter = Cast<APSCharacter>(FoundCharacters[0]);
		if (CachedCharacter)
		{
			USceneComponent* ListenerComponent = Cast<USceneComponent>(CachedCharacter->GetRootComponent());
			if (ListenerComponent)
			{
				SetAudioListenerOverride(ListenerComponent, FVector::ZeroVector, FRotator::ZeroRotator);
			}
		}
	}

	// cache Camera
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SelectedCameraClass, FoundCameras);
	if (FoundCameras.Num() > 0)
	{
		CachedCameraActor = Cast<ACameraActor>(FoundCameras[0]);
	}
	if (CachedHUD && PSScoreController)
	{
		PSScoreController->InitializeCachedHUD(CachedHUD);
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
	if (CachedCharacter)
	{
		ReverseEnemyDirections();
		FVector CharacterLocation = CachedCharacter->GetActorLocation();
		FVector CameraOffset(-275.0f, 575.0f, 300.0f);
		FVector CameraLocation = CharacterLocation + CameraOffset;

		if (CachedCameraActor)
		{
			CachedCameraActor->SetActorLocation(CameraLocation);
			SetViewTargetWithBlend(CachedCameraActor, 3.0f);
		}
	}


	if (PSScoreController && CachedHUD && PSScoreController)
	{
		int32 TotalScore = PSScoreController->GetTotalScore();
		TMap<EPickupItemType, int32> ItemCounts = PSScoreController->GetItemPickupCounts();
		CachedHUD->SetHUDWidgetVisibility(false);
		CachedHUD->DisplayScoreScreen(ItemCounts, TotalScore);
	}

	StopMusic();
}

void APSPlayerController::StartScoreScreenProcess()
{
	Cleanup();

	FVector ScaleSpawnLocation = CachedCharacter->GetActorLocation();
	ScaleSpawnLocation.Z = 600;

	if (PSScoreController)
	{
		PSScoreController->StartSpawningScales(ScaleSpawnLocation, 3.0f, 0.1f, FVector(100.0f, 100.0f, 0.0f));
	}
}

void APSPlayerController::ReverseEnemyDirections()
{
	FVector CharacterLocation = CachedCharacter->GetActorLocation();

	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APSEnemy::StaticClass(), EnemyActors);

	for (AActor* Actor : EnemyActors)
	{
		if (APSEnemy* Enemy = Cast<APSEnemy>(Actor))
		{
			Enemy->OnCharacterDeath();
			FVector EnemyLocation = Enemy->GetActorLocation();
			FVector ToCharacter = FVector(CharacterLocation.X, CharacterLocation.Y, EnemyLocation.Z) - EnemyLocation;
			FVector OppositeDirection = -ToCharacter.GetSafeNormal();

			Enemy->InitializeDirection(OppositeDirection);
		}
	}
}

void APSPlayerController::Cleanup()
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
}
