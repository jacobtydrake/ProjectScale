// Written by Jacob Drake - 2023


#include "PSScoreScreen.h"
#include "Components/TextBlock.h"
#include "ProjectScale/Actors/PSPickupItem.h"

void UPSScoreScreen::SetTotalScore(int32 NewTotalScore)
{
    TotalScore = NewTotalScore;
    if (TotalScoreText)
    {
        TotalScoreText->SetText(FText::AsNumber(TotalScore));
    }
}

void UPSScoreScreen::SetItemPickupCounts(const TMap<EPickupItemType, int32>& NewItemPickupCounts)
{
    ItemPickupCounts = NewItemPickupCounts;

    if (OrangeScaleCount && OrangeScaleScore && BlueScaleCount && BlueScaleScore)
    {
        int32 OrangeScaleCountValue = ItemPickupCounts.Contains(EPickupItemType::OrangeScale) ? ItemPickupCounts[EPickupItemType::OrangeScale] : 0;
        int32 BlueScaleCountValue = ItemPickupCounts.Contains(EPickupItemType::BlueScale) ? ItemPickupCounts[EPickupItemType::BlueScale] : 0;

        OrangeScaleCount->SetText(FText::AsNumber(OrangeScaleCountValue));
        BlueScaleCount->SetText(FText::AsNumber(BlueScaleCountValue));

        // respective scores should really be calculated and kept in scorecontroller but oh well
        OrangeScaleScore->SetText(FText::AsNumber(OrangeScaleCountValue));
        BlueScaleScore->SetText(FText::AsNumber(BlueScaleCountValue * 2));
    }
}
