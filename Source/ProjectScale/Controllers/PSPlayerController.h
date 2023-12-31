// Written by Jacob Drake - 2023

#pragma once

#include "GameFramework/PlayerController.h"
#include "PSPlayerController.generated.h"

class UInputMappingContext;
class APSHUD;
class UPSScoreController;
class ACameraActor;
class APSCharacter;
class USoundBase;
/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
public:
	// TODO: Handle this only via c++
	/* Triggered from blueprint */
	UFUNCTION(BlueprintCallable)
	void OnPauseButtonPressed();

	// called from PSCharacter on death
	UFUNCTION()
	void OnCharacterDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void StopMusic();

	UFUNCTION(BlueprintCallable)
	void StartScoreScreenProcess();

	UFUNCTION()
	void ReverseEnemyDirections() const;

	/* Getters */
	UFUNCTION()
	APSHUD* GetPSHUD() const { return CachedHUD; }
	UFUNCTION()
	UPSScoreController* GetPSScoreController() const { return PSScoreController;  }


protected:

	void Cleanup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerCharacterMappingContext;

	UPROPERTY()
	TObjectPtr<APSHUD> CachedHUD;

	UPROPERTY()
	TObjectPtr<UPSScoreController> PSScoreController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<ACameraActor> SelectedCameraClass;

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> ScaleSpawnSoundforController;

private:
	TObjectPtr<APSCharacter> CachedCharacter;
	TObjectPtr<ACameraActor> CachedCameraActor;

};
