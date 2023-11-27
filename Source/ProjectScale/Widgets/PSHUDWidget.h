// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSHUDWidget.generated.h"

class UImage;
class UPSPowerupWidget;


/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateHealth(int32 NewHealth);
	UFUNCTION()
	void UpdateAttackWidget(const float Time);
	UFUNCTION()
	void UpdateSpeedWidget(const float Time);


protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSPowerupWidget> AttackPowerup;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSPowerupWidget> SpeedPowerup;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image3;
	
};
