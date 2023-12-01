// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSGenericButton.generated.h"

class UButton;
class USoundBase;
/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSGenericButton : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/* Overrides */
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> OnHoverSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> OnClickSound;
};
