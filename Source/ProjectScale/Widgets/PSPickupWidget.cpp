// Written by Jacob Drake - 2023


#include "PSPickupWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

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
