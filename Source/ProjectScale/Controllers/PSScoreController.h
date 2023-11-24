// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PSScoreController.generated.h"

enum class EPickupItemType : uint8;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSScoreController : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void AddItemToScore(const EPickupItemType ItemType);
	UFUNCTION()
	int32 GetTotalScore() const;
	UFUNCTION()
	TMap<EPickupItemType, int32> GetItemPickupCounts() const;



private:

	int32 TotalScore = 0;

	TMap<EPickupItemType, int32> ItemPickupCounts;

	int32 CalculateScoreForItem(EPickupItemType ItemType) const;
	
};
