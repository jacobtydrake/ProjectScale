// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSPickupWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSPickupWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetText(const FString NewText);

	void PlayPickupAnimation();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RaiseAndFadeAnim;

};
