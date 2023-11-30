// Written by Jacob Drake - 2023


#include "PSPickupWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"


void UPSPickupWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (UGameplayStatics::IsGamePaused(GetWorld()))
    {
        if (RaiseAndFadeAnim && IsAnimationPlaying(RaiseAndFadeAnim))
        {
            PauseAnimation(RaiseAndFadeAnim);
        }
    }
    else
    {
        if (RaiseAndFadeAnim && !IsAnimationPlaying(RaiseAndFadeAnim) && GetAnimationCurrentTime(RaiseAndFadeAnim) > 0)
        {
            PlayAnimation(RaiseAndFadeAnim, GetAnimationCurrentTime(RaiseAndFadeAnim));
        }
    }
}

void UPSPickupWidget::SetText(FString NewText)
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::FromString(NewText));
	}
}

void UPSPickupWidget::PlayPickupAnimation()
{
	if (RaiseAndFadeAnim)
	{
		PlayAnimation(RaiseAndFadeAnim);
	}
}
