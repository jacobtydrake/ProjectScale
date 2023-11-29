// Written by Jacob Drake - 2023


#include "PSPowerupWidget.h"
#include "Components/RadialSlider.h"
#include "Kismet/GameplayStatics.h"

void UPSPowerupWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::Collapsed);
}

void UPSPowerupWidget::InitializeTimer(float Duration)
{
    TotalTime = Duration;
    RemainingTime = Duration;
    SetVisibility(ESlateVisibility::Visible);

    if (PowerupRadialSlider)
    {
        PowerupRadialSlider->SetValue(1.0f);
    }
}

void UPSPowerupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!UGameplayStatics::IsGamePaused(GetWorld()) && RemainingTime > 0)
    {
        RemainingTime -= InDeltaTime;
        if (PowerupRadialSlider)
        {
            PowerupRadialSlider->SetValue(RemainingTime / TotalTime);
        }
    }
    else if (RemainingTime <= 0)
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}