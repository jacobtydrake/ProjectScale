// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSGenericButton.generated.h"

class UButton;

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

	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;
};
