// Written by Jacob Drake - 2023


#include "PSHUDWidget.h"
#include "Components/Image.h"
#include "ProjectScale/Widgets/PSPowerupWidget.h"

void UPSHUDWidget::UpdateHealth(int32 NewHealth)
{
    if (NewHealth > CurrentHealthIndex)
    {
        PlayHealthGainAnimation(CurrentHealthIndex);
    }
    else if (NewHealth < CurrentHealthIndex)
    {
        PlayHealthLossAnimation(NewHealth);
    }
    CurrentHealthIndex = NewHealth;
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

void UPSHUDWidget::PlayHealthGainAnimation(int32 HealthIndex)
{
    CurrentAnimationHealthIndex = HealthIndex;
    CurrentAnimationSpriteIndex = 0;
    UpdateHealthGainSprite();
}

void UPSHUDWidget::PlayHealthLossAnimation(int32 HealthIndex)
{
    CurrentAnimationHealthIndex = HealthIndex;
    CurrentAnimationSpriteIndex = 0;
    UpdateHealthLossSprite();
}

void UPSHUDWidget::UpdateHealthGainSprite()
{
    if (!HealthGainSprites.IsValidIndex(CurrentAnimationSpriteIndex)) return;

    UImage* TargetImage = GetImageForHealthIndex(CurrentAnimationHealthIndex);
    if (TargetImage)
    {
        TargetImage->SetBrushFromTexture(HealthGainSprites[CurrentAnimationSpriteIndex]);
    }
    if (CurrentAnimationSpriteIndex + 1 < HealthGainSprites.Num())
    {
        CurrentAnimationSpriteIndex++;
        GetWorld()->GetTimerManager().SetTimer(HealthGainTimerHandle, this, &UPSHUDWidget::UpdateHealthGainSprite, AnimationDelay, false);
    }
}

void UPSHUDWidget::UpdateHealthLossSprite()
{
    if (!HealthLossSprites.IsValidIndex(CurrentAnimationSpriteIndex)) return;

    UImage* TargetImage = GetImageForHealthIndex(CurrentAnimationHealthIndex);
    if (TargetImage)
    {
        TargetImage->SetBrushFromTexture(HealthLossSprites[CurrentAnimationSpriteIndex]);
    }

    if (CurrentAnimationSpriteIndex + 1 < HealthLossSprites.Num())
    {
        CurrentAnimationSpriteIndex++;
        GetWorld()->GetTimerManager().SetTimer(HealthLossTimerHandle, this, &UPSHUDWidget::UpdateHealthLossSprite, AnimationDelay, false);
    }
}

TObjectPtr<UImage> UPSHUDWidget::GetImageForHealthIndex(int32 HealthIndex)
{
    switch (HealthIndex)
    {
    case 0: return Image1;
    case 1: return Image2;
    case 2: return Image3;
    default: return nullptr;
    }
}
