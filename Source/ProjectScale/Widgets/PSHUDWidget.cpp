// Written by Jacob Drake - 2023


#include "PSHUDWidget.h"
#include "Components/Image.h"
#include "ProjectScale/Widgets/PSPowerupWidget.h"

void UPSHUDWidget::UpdateHealth(int32 NewHealth)
{
	if (Image1 && Image2 && Image3)
	{
		Image1->SetVisibility(NewHealth >= 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		Image2->SetVisibility(NewHealth >= 2 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		Image3->SetVisibility(NewHealth >= 3 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UPSHUDWidget::UpdateAttackWidget(const float Time)
{
	if (AttackPowerup)
	{
		AttackPowerup->InitializeTimer(Time);
	}
}

void UPSHUDWidget::UpdateSpeedWidget(const float Time)
{
	if (SpeedPowerup)
	{
		SpeedPowerup->InitializeTimer(Time);
	}
}
