// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSScoreScreen.generated.h"

enum class EPickupItemType : uint8;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSScoreScreen : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION()
    void SetTotalScore(int32 NewTotalScore);

    UFUNCTION(BlueprintCallable, Category = "Score")
    void SetItemPickupCounts(const TMap<EPickupItemType, int32>& NewItemPickupCounts);


protected:

    UPROPERTY()
    int32 TotalScore;
    UPROPERTY()
    TMap<EPickupItemType, int32> ItemPickupCounts;

    /* Widget Bindings */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TotalScoreText;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> OrangeScaleCount;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> OrangeScaleScore;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BlueScaleCount;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BlueScaleScore;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PurpleScaleCount;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PurpleScaleScore;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BlackScaleCount;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BlackScaleScore;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> GoldScaleCount;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> GoldScaleScore;
};
