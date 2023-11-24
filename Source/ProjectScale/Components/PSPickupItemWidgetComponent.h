// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PSPickupItemWidgetComponent.generated.h"

class UPSPickupWidget;
enum class EPickupItemType: uint8;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSPickupItemWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UPSPickupItemWidgetComponent();


	void ActivePickupItemWidget(const EPickupItemType ItemType);

protected:

	void BeginPlay() override;

	void InitializeWidget();


	TSubclassOf<UPSPickupWidget> PSPickupWidgetClass;
	TObjectPtr<UPSPickupWidget> WidgetInst;
	
};
