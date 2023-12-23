// Written by Jacob Drake - 2023

#pragma once

#include "Components/WidgetComponent.h"
#include "PSCharacterWidgetComponent.generated.h"

class UPSCharacterWidget;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	UPSCharacterWidgetComponent();


protected:

	/* Overrides */
	void BeginPlay() override;

	void InitializeWidget();
	
	TSubclassOf<UPSCharacterWidget> PSCharacterWidgetClass;
	TObjectPtr<UPSCharacterWidget> WidgetInst;
	
};
