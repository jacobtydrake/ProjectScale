// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSPowerupWidget.generated.h"

class URadialSlider;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSPowerupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void InitializeTimer(float Duration);

protected:
    /* Overrides */
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


    UPROPERTY(meta = (BindWidget))
    TObjectPtr<URadialSlider> PowerupRadialSlider;

    float TotalTime;
    float RemainingTime;
	
};
