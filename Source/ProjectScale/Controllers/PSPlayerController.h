

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PSPlayerController.generated.h"

class UInputMappingContext;
class APSHUD;
class UPSScoreController;

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

	/* Getters */
	UFUNCTION()
	APSHUD* GetPSHUD() const { return CachedHUD; }
	UFUNCTION()
	UPSScoreController* GetPSScoreController() const { return PSScoreController;  }


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerCharacterMappingContext;

	UPROPERTY()
	TObjectPtr<APSHUD> CachedHUD;

	UPROPERTY()
	TObjectPtr<UPSScoreController> PSScoreController;

};
