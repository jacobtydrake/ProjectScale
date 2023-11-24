// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PSHUD.generated.h"

class UPSHUDWidget;
class UPSPauseScreen;
class UPSScoreScreen;

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

	UFUNCTION()
	void DisplayScoreScreen(const TMap<EPickupItemType, int32>& ItemCounts, int32 TotalScore);

private:
	TSubclassOf<UPSHUDWidget> HUDWidgetClass;
	TSubclassOf<UPSPauseScreen> PauseWidgetClass;
	TSubclassOf<UPSScoreScreen> ScoreScreenWidgetClass;

	TObjectPtr<UPSHUDWidget> HUDWidgetInstance;
	TObjectPtr<UPSPauseScreen> PauseWidgetInstance;
	TObjectPtr<UPSScoreScreen> ScoreScreenWidgetInstance;

	bool bIsPauseScreenVisible{ false };
};
