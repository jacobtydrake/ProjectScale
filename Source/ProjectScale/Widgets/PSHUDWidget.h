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
	void PlayHealthGainAnimation(int32 HealthIndex);
	void PlayHealthLossAnimation(int32 HealthIndex);
	/**/
	void UpdateHealthGainSprite();
	void UpdateHealthLossSprite();

	TObjectPtr<UImage> GetImageForHealthIndex(int32 HealthIndex);



	/**/
	UPROPERTY(EditAnywhere, Category = "Health Animation")
	TArray<TObjectPtr<UTexture2D>> HealthGainSprites;

	UPROPERTY(EditAnywhere, Category = "Health Animation")
	TArray<TObjectPtr<UTexture2D>> HealthLossSprites;

	/**/
	float AnimationDelay{ 0.1f }; 
	int32 CurrentHealthIndex{ 3 };
	bool bIsAnimatingHealthGain{ false };

	int32 CurrentAnimationHealthIndex;
	int32 CurrentAnimationSpriteIndex;

	/**/
	FTimerHandle HealthGainTimerHandle;
	FTimerHandle HealthLossTimerHandle;

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
