// Written by Jacob Drake - 2023


#include "PSScoreController.h"
#include "ProjectScale/Actors/PSPickupItem.h"

void UPSScoreController::AddItemToScore(EPickupItemType ItemType)
{
    int32 ScoreToAdd = CalculateScoreForItem(ItemType);
    TotalScore += ScoreToAdd;
    ItemPickupCounts.FindOrAdd(ItemType)++;
}

int32 UPSScoreController::GetTotalScore() const
{
    return TotalScore;
}

TMap<EPickupItemType, int32> UPSScoreController::GetItemPickupCounts() const
{
    return ItemPickupCounts;
}

int32 UPSScoreController::CalculateScoreForItem(EPickupItemType ItemType) const
{
    switch (ItemType)
    {
    case EPickupItemType::BlueScale:
        return 1;
    case EPickupItemType::OrangeScale:
        return 2; 
    case EPickupItemType::PurpleScale:
        return 3;
    case EPickupItemType::BlackScale:
        return 10;
    case EPickupItemType::GoldScale:
        return 20;
    default:
        return 0;
    }
}
