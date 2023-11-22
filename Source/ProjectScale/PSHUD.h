// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PSHUD.generated.h"

class UPSHUDWidget;
class UPSPauseScreen;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSHUD : public AHUD
{
	GENERATED_BODY()

	APSHUD();

protected:
	/* Overrides */
	virtual void BeginPlay() override;

public:
	/**/
	UFUNCTION(BlueprintCallable)
	void InitializeMainLevelUI();

	/**/
	UFUNCTION()
	void TogglePauseScreen();

	UFUNCTION()
	void UpdateHealthWidget(int32 NewHealth);

private:
	TSubclassOf<UPSHUDWidget> HUDWidgetClass;
	TSubclassOf<UPSPauseScreen> PauseWidgetClass;

	TObjectPtr<UPSHUDWidget> HUDWidgetInstance;
	TObjectPtr<UPSPauseScreen> PauseWidgetInstance;

	bool bIsPauseScreenVisible{ false };
};
