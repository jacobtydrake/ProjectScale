// Written by Jacob Drake - 2023


#include "PSScoreScreen.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "ProjectScale/Actors/PSPickupItem.h"
#include "ProjectScale/PSHUD.h"
#include "Components/Border.h"

void UPSScoreScreen::NativePreConstruct()
{
    Super::NativePreConstruct();
}

void UPSScoreScreen::NativeConstruct()
{
    if (OrangeScaleBox) ScaleBoxes.Add(OrangeScaleBox);
    if (BlueScaleBox) ScaleBoxes.Add(BlueScaleBox);
    if (PurpleScaleBox) ScaleBoxes.Add(PurpleScaleBox);
    if (BlackScaleBox) ScaleBoxes.Add(BlackScaleBox);
    if (GoldScaleBox) ScaleBoxes.Add(GoldScaleBox);
    if (FinalScoreBox) ScaleBoxes.Add(FinalScoreBox);

    for (UHorizontalBox* Box : ScaleBoxes)
    {
        if (Box)
        {
            Box->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

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

    auto CreateTextWithAppend = [](int32 Value, const FString& AppendText) -> FText {
        return FText::FromString(FString::Printf(TEXT("%d %s"), Value, *AppendText));
        };

    int32 OrangeScaleCountValue = ItemPickupCounts.Contains(EPickupItemType::OrangeScale) ? ItemPickupCounts[EPickupItemType::OrangeScale] : 0;
    int32 BlueScaleCountValue = ItemPickupCounts.Contains(EPickupItemType::BlueScale) ? ItemPickupCounts[EPickupItemType::BlueScale] : 0;
    int32 PurpleScaleCountValue = ItemPickupCounts.Contains(EPickupItemType::PurpleScale) ? ItemPickupCounts[EPickupItemType::PurpleScale] : 0;
    int32 BlackScaleCountValue = ItemPickupCounts.Contains(EPickupItemType::BlackScale) ? ItemPickupCounts[EPickupItemType::BlackScale] : 0;
    int32 GoldScaleCountValue = ItemPickupCounts.Contains(EPickupItemType::GoldScale) ? ItemPickupCounts[EPickupItemType::GoldScale] : 0;

    OrangeScaleCount->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::OrangeScale), TEXT("Collected")));
    OrangeScaleScore->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::OrangeScale), TEXT("Points")));

    BlueScaleCount->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::BlueScale), TEXT("Collected")));
    BlueScaleScore->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::BlueScale) * 2, TEXT("Points")));

    PurpleScaleCount->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::PurpleScale), TEXT("Collected")));
    PurpleScaleScore->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::PurpleScale) * 3, TEXT("Points")));

    BlackScaleCount->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::BlackScale), TEXT("Collected")));
    BlackScaleScore->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::BlackScale) * 10, TEXT("Points")));

    GoldScaleCount->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::GoldScale), TEXT("Collected")));
    GoldScaleScore->SetText(CreateTextWithAppend(ItemPickupCounts.FindOrAdd(EPickupItemType::GoldScale) * 20, TEXT("Points")));
}

void UPSScoreScreen::SetBoxVisibility(const int32 CurrentIndex)
{
    if (ScaleBoxes.IsValidIndex(CurrentIndex))
    {
        UHorizontalBox* Box = ScaleBoxes[CurrentIndex];
        if (Box)
        {
            Box->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }
    }
}
