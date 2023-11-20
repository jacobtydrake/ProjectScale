

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PSPlayerController.generated.h"

class UInputMappingContext;
class APSHUD;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerCharacterMappingContext;

private:

	TObjectPtr<APSHUD> CachedHUD;
};
