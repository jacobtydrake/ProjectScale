// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSHUDWidget.generated.h"

class UImage;


/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateHealth(int32 NewHealth);


protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image3;
	
};
