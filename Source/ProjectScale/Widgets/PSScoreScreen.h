// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSScoreScreen.generated.h"

enum class EPickupItemType : uint8;
class UTextBlock;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSScoreScreen : public UUserWidget
{
	GENERATED_BODY()

protected:

    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

public:

    UFUNCTION()
    void SetTotalScore(int32 NewTotalScore);

    UFUNCTION(BlueprintCallable, Category = "Score")
    void SetItemPickupCounts(const TMap<EPickupItemType, int32>& NewItemPickupCounts);

    UFUNCTION()
    void SetBoxVisibility(const int32 CurrentIndex);

protected:

    UPROPERTY()
    int32 TotalScore;
    UPROPERTY()
    TMap<EPickupItemType, int32> ItemPickupCounts;
    UPROPERTY()
    TArray<TObjectPtr<UHorizontalBox>> ScaleBoxes;

    /********** Start Widget Bindings **********/
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TotalScoreText;

    /* Containing Horizontal Boxes */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> OrangeScaleBox;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> BlueScaleBox;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> PurpleScaleBox;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> BlackScaleBox;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> GoldScaleBox;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> FinalScoreBox;

    /* Scale Scores and Counts */
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
    /********** End Widget Bindings **********/
};
