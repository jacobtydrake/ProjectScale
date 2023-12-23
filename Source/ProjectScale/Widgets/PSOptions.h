// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSOptions.generated.h"

class UComboBoxString;
class UPSGenericButton;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSOptions : public UUserWidget
{
	GENERATED_BODY()
protected:
	/* Overrides */
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnVsyncChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnApplySettings();

	/* Widget Bindings */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> WindowModeOptions;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> VSyncOptions;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> ResolutionOptions;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPSGenericButton> ApplyButton;
	
	bool bVSyncEnabled;
	FIntPoint SelectedResolution;
};
