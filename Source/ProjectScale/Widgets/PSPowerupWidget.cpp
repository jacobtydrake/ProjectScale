// Written by Jacob Drake - 2023


#include "PSPowerupWidget.h"
#include "UMG.h"
#include "Components/RadialSlider.h"

void UPSPowerupWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    SetVisibility(ESlateVisibility::Collapsed);
}

void UPSPowerupWidget::InitializeTimer(float Duration)
{
    TotalTime = Duration;
    RemainingTime = Duration;
    SetVisibility(ESlateVisibility::Visible);
}

void UPSPowerupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (RemainingTime > 0)
    {
        RemainingTime -= InDeltaTime;
        if (PowerupRadialSlider)
        {
            PowerupRadialSlider->SetValue(RemainingTime / TotalTime);
        }
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}
